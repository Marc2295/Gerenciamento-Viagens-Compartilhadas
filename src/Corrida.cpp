#include "Corrida.hpp"
#include <cmath>

InfoCorrida::InfoCorrida() 
    : numDemandas(0), capacidade(10), distanciaTotal(0.0), 
      tempoInicio(0.0), processada(false) {
    demandasIds = new int[capacidade];
}

InfoCorrida::~InfoCorrida() {
    delete[] demandasIds;
}

void InfoCorrida::adicionarDemanda(int idDemanda) {
    if (numDemandas >= capacidade) {
        // Redimensiona se necessário
        int novaCapacidade = capacidade * 2;
        int* novoArray = new int[novaCapacidade];
        for (int i = 0; i < numDemandas; i++) {
            novoArray[i] = demandasIds[i];
        }
        delete[] demandasIds;
        demandasIds = novoArray;
        capacidade = novaCapacidade;
    }
    demandasIds[numDemandas++] = idDemanda;
}

void InfoCorrida::calcularRota(Demanda* demandas, double gama) {
    if (numDemandas == 0) return;

    if (numDemandas == 1) {
        // Corrida individual
        int id = demandasIds[0];
        distanciaTotal = demandas[id].calcularDistanciaViagem();
        tempoInicio = demandas[id].getTempoSolicitacao();
        return;
    }

    // Corrida compartilhada: origem primeira demanda -> coletas -> entregas
    double dist = 0.0;
    Coordenada posAtual = demandas[demandasIds[0]].getOrigem();

    // Fase 1: Coletas (visitar todas origens)
    for (int i = 1; i < numDemandas; i++) {
        Coordenada proxOrigem = demandas[demandasIds[i]].getOrigem();
        dist += calcularDistancia(posAtual, proxOrigem);
        posAtual = proxOrigem;
    }

    // Fase 2: Entregas (visitar todos destinos)
    for (int i = 0; i < numDemandas; i++) {
        Coordenada destino = demandas[demandasIds[i]].getDestino();
        dist += calcularDistancia(posAtual, destino);
        posAtual = destino;
    }

    distanciaTotal = dist;
    tempoInicio = demandas[demandasIds[0]].getTempoSolicitacao();
}

double InfoCorrida::calcularEficiencia(Demanda* demandas) const {
    if (numDemandas <= 1) return 100.0;

    // Distância do trecho principal (primeira origem -> primeiro destino)
    double distPrincipal = demandas[demandasIds[0]].calcularDistanciaViagem();

    if (distanciaTotal == 0.0) return 0.0;

    // Eficiência = (distância principal / distância total) * 100
    return (distPrincipal / distanciaTotal) * 100.0;
}