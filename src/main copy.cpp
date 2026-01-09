#include "Demanda.hpp"
#include "Escalonador.hpp"
#include "Corrida.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

/**
 * @brief Verifica se duas demandas podem ser combinadas
 * @param d1 Primeira demanda
 * @param d2 Segunda demanda
 * @param alfa Distância máxima entre origens
 * @param beta Distância máxima entre destinos
 * @return true se podem ser combinadas
 */
bool podeCombinar(const Demanda& d1, const Demanda& d2, double alfa, double beta) {
    double distOrigens = d1.distanciaOrigem(d2.getOrigem());
    double distDestinos = d1.distanciaDestino(d2.getDestino());

    return (distOrigens <= alfa) && (distDestinos <= beta);
}

/**
 * @brief Verifica se nova demanda pode ser adicionada ao grupo
 * @param demandas Array de todas demandas
 * @param grupo Array de índices das demandas no grupo atual
 * @param tamGrupo Tamanho do grupo
 * @param novaDemanda Índice da demanda a ser testada
 * @param alfa Critério de distância entre origens
 * @param beta Critério de distância entre destinos
 * @return true se pode adicionar
 */
bool podeAdicionarAoGrupo(Demanda* demandas, int* grupo, int tamGrupo, 
                          int novaDemanda, double alfa, double beta) {
    // Verifica se a nova demanda satisfaz critérios com TODAS do grupo
    for (int i = 0; i < tamGrupo; i++) {
        if (!podeCombinar(demandas[grupo[i]], demandas[novaDemanda], alfa, beta)) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Calcula eficiência de uma corrida compartilhada
 * @param demandas Array de demandas
 * @param grupo Índices das demandas na corrida
 * @param tamGrupo Número de demandas
 * @return Eficiência em porcentagem
 */
double calcularEficienciaGrupo(Demanda* demandas, int* grupo, int tamGrupo) {
    if (tamGrupo <= 1) return 100.0;

    // Distância do trecho principal (soma das demandas avulsas)
    double distPrincipal = 0;
    for (int i=0; i < tamGrupo; i++){
        distPrincipal += demandas[grupo[i]].calcularDistanciaViagem();
    }
    // Calcular distância total da rota: origem[0] -> origens[1..n] -> destinos[0..n]
    double distTotal = 0.0;
    Coordenada posAtual = demandas[grupo[0]].getOrigem();

    // Fase de coletas: visitar origens das demais demandas
    for (int i = 1; i < tamGrupo; i++) {
        Coordenada proxOrigem = demandas[grupo[i]].getOrigem();
        distTotal += calcularDistancia(posAtual, proxOrigem);
        posAtual = proxOrigem;
    }

    // Fase de entregas: visitar todos os destinos
    for (int i = 0; i < tamGrupo; i++) {
        Coordenada destino = demandas[grupo[i]].getDestino();
        distTotal += calcularDistancia(posAtual, destino);
        posAtual = destino;
    }

    if (distTotal == 0.0) return 0.0;

    return (distPrincipal / distTotal);
}

int main() {
    // Leitura dos parâmetros
    int eta;           // Capacidade dos veículos
    double gama;       // Velocidade dos veículos
    double delta;      // Intervalo temporal máximo
    double alfa;       // Distância máxima entre origens
    double beta;       // Distância máxima entre destinos
    double lambda;     // Eficiência mínima

    cin >> eta >> gama >> delta >> alfa >> beta >> lambda;

    // Leitura das demandas
    int numDemandas;
    cin >> numDemandas;

    Demanda* demandas = new Demanda[numDemandas];

    for (int i = 0; i < numDemandas; i++) {
        int id;
        double tempo;
        Coordenada origem, destino;

        cin >> id >> tempo >> origem.x >> origem.y >> destino.x >> destino.y;
        demandas[i] = Demanda(id, tempo, origem, destino);
    }

    // Array para controlar quais demandas já foram atribuídas a corridas
    bool* demandaAtribuida = new bool[numDemandas];
    for (int i = 0; i < numDemandas; i++) {
        demandaAtribuida[i] = false;
    }

    // Array para armazenar corridas criadas
    InfoCorrida* corridas = new InfoCorrida[numDemandas]; // No máximo numDemandas corridas
    int numCorridas = 0;

    // Escalonador para eventos
    Escalonador escalonador;

    // ========== FASE 1: COMBINAÇÃO DE CORRIDAS ==========

    for (int i = 0; i < numDemandas; i++) {
        // Pular se demanda já foi atribuída a alguma corrida
        if (demandaAtribuida[i]) {
            continue;
        }

        // Criar novo grupo de corrida começando com demanda i
        int* grupo = new int[eta];  // Máximo eta passageiros
        grupo[0] = i;
        int tamGrupo = 1;
        demandaAtribuida[i] = true;

        double tempoBase = demandas[i].getTempoSolicitacao();

        // Tentar adicionar outras demandas ao grupo
        for (int j = i + 1; j < numDemandas; j++) {
            // Pular se demanda já foi atribuída
            if (demandaAtribuida[j]) {
                continue;
            }

            // Verificar se grupo já está cheio
            if (tamGrupo >= eta) {
                break;
            }

            // Verificar intervalo temporal (critério delta)
            double diffTempo = demandas[j].getTempoSolicitacao() - tempoBase;
            if (diffTempo >= delta) {
                break;  // Demandas estão ordenadas por tempo, pode parar
            }

            // Verificar se pode combinar com todas do grupo (critérios alfa e beta)
            if (!podeAdicionarAoGrupo(demandas, grupo, tamGrupo, j, alfa, beta)) {
                continue;  // Não pode combinar, tenta próxima
            }

            // Adicionar temporariamente ao grupo
            grupo[tamGrupo] = j;
            tamGrupo++;

            // Verificar eficiência
            double eficiencia = calcularEficienciaGrupo(demandas, grupo, tamGrupo);

            if (eficiencia < lambda) {
                // Eficiência insuficiente, remover última demanda
                tamGrupo--;
                // Não interrompe busca - tenta próximas demandas
            } else {
                // Aceita a demanda no grupo
                demandaAtribuida[j] = true;
            }
        }

        // Criar corrida com o grupo formado
        InfoCorrida& corrida = corridas[numCorridas];
        for (int k = 0; k < tamGrupo; k++) {
            corrida.adicionarDemanda(grupo[k]);

            // Atualizar estado da demanda
            if (tamGrupo == 1) {
                demandas[grupo[k]].setEstado(INDIVIDUAL);
            } else {
                demandas[grupo[k]].setEstado(COMBINADA);
            }
        }

        corrida.calcularRota(demandas, gama);

        // Escalonar eventos de coleta e entrega
        double tempoAtual = corrida.tempoInicio;
        Coordenada posAtual = demandas[corrida.demandasIds[0]].getOrigem();

        // Eventos de coleta
        for (int k = 0; k < corrida.numDemandas; k++) {
            Coordenada destColeta = demandas[corrida.demandasIds[k]].getOrigem();
            double distancia = calcularDistancia(posAtual, destColeta);
            tempoAtual += distancia / gama;

            Evento eventoColeta = {tempoAtual, COLETA, numCorridas, 
                                   corrida.demandasIds[k], k};
            escalonador.insereEvento(eventoColeta);

            posAtual = destColeta;
        }

        // Eventos de entrega
        for (int k = 0; k < corrida.numDemandas; k++) {
            Coordenada destEntrega = demandas[corrida.demandasIds[k]].getDestino();
            double distancia = calcularDistancia(posAtual, destEntrega);
            tempoAtual += distancia / gama;

            Evento eventoEntrega = {tempoAtual, ENTREGA, numCorridas, 
                                    corrida.demandasIds[k], k};
            escalonador.insereEvento(eventoEntrega);

            posAtual = destEntrega;
        }

        numCorridas++;
        delete[] grupo;
    }

    // ========== FASE 2: PROCESSAMENTO DE EVENTOS E SAÍDA ==========

    // Array para controlar quando cada corrida foi concluída
    double* tempoConclusao = new double[numCorridas];
    int* paradasPorCorrida = new int[numCorridas];
    bool* corridaConcluida = new bool[numCorridas];

    for (int i = 0; i < numCorridas; i++) {
        tempoConclusao[i] = 0.0;
        paradasPorCorrida[i] = 0;
        corridaConcluida[i] = false;
    }

    // Arrays para armazenar paradas de cada corrida
    Coordenada** paradasCorridas = new Coordenada*[numCorridas];
    for (int i = 0; i < numCorridas; i++) {
        // Máximo de paradas: numDemandas coletas + numDemandas entregas
        paradasCorridas[i] = new Coordenada[corridas[i].numDemandas * 2];
    }

    // Processar eventos
    while (!escalonador.vazio()) {
        Evento evento = escalonador.retiraProximoEvento();
        int idCorrida = evento.idCorrida;

        // Registrar parada
        Coordenada parada;
        if (evento.tipo == COLETA) {
            parada = demandas[evento.idDemanda].getOrigem();
        } else {
            parada = demandas[evento.idDemanda].getDestino();
        }

        paradasCorridas[idCorrida][paradasPorCorrida[idCorrida]] = parada;
        paradasPorCorrida[idCorrida]++;

        // Atualizar tempo de conclusão
        tempoConclusao[idCorrida] = evento.tempo;

        // Verificar se é o último evento da corrida (última entrega)
        if (evento.tipo == ENTREGA && 
            evento.indiceTrecho == corridas[idCorrida].numDemandas - 1) {
            corridaConcluida[idCorrida] = true;
        }
    }

    // Imprimir resultados (ordem de conclusão das corridas)
    cout << fixed << setprecision(2);

    for (int i = 0; i < numCorridas; i++) {
        if (corridaConcluida[i]) {
            cout << tempoConclusao[i] << " "
                 << corridas[i].distanciaTotal << " "
                 << paradasPorCorrida[i];

            for (int j = 0; j < paradasPorCorrida[i]; j++) {
                cout << " " << paradasCorridas[i][j].x 
                     << " " << paradasCorridas[i][j].y;
            }
            cout << endl;
        }
    }

    // Liberar memória
    delete[] demandas;
    delete[] demandaAtribuida;
    delete[] corridas;
    delete[] tempoConclusao;
    delete[] paradasPorCorrida;
    delete[] corridaConcluida;

    for (int i = 0; i < numCorridas; i++) {
        delete[] paradasCorridas[i];
    }
    delete[] paradasCorridas;

    return 0;
}