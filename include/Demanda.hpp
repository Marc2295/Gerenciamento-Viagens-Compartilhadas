#ifndef DEMANDA_HPP
#define DEMANDA_HPP

/**
 * @file Demanda.hpp
 * @brief TAD Demanda - Representa uma solicitação de corrida
 * 
 * Armazena informações sobre uma demanda de transporte incluindo
 * origem, destino, tempo de solicitação e estado atual
 */

struct Coordenada {
    double x;
    double y;
};

enum EstadoDemanda {
    DEMANDADA,      // Corrida solicitada, aguardando processamento
    INDIVIDUAL,     // Corrida não compartilhada
    COMBINADA,      // Corrida compartilhada com outros passageiros
    CONCLUIDA       // Corrida finalizada
};

class Demanda {
private:
    int id;
    double tempoSolicitacao;
    Coordenada origem;
    Coordenada destino;
    EstadoDemanda estado;
    int corridaAssociada;  // ID da corrida à qual pertence

public:
    /**
     * @brief Construtor padrão
     */
    Demanda();

    /**
     * @brief Construtor com parâmetros
     * @param id Identificador único da demanda
     * @param tempo Tempo de solicitação
     * @param orig Coordenadas da origem
     * @param dest Coordenadas do destino
     */
    Demanda(int id, double tempo, Coordenada orig, Coordenada dest);

    /**
     * @brief Destrutor
     */
    ~Demanda();

    // Getters
    int getId() const;
    double getTempoSolicitacao() const;
    Coordenada getOrigem() const;
    Coordenada getDestino() const;
    EstadoDemanda getEstado() const;
    int getCorridaAssociada() const;

    // Setters
    void setEstado(EstadoDemanda novoEstado);
    void setCorridaAssociada(int idCorrida);

    /**
     * @brief Calcula distância euclidiana entre origem e destino
     * @return Distância em unidades de espaço
     */
    double calcularDistanciaViagem() const;

    /**
     * @brief Calcula distância entre origem desta demanda e outra coordenada
     * @param coord Coordenada de referência
     * @return Distância euclidiana
     */
    double distanciaOrigem(const Coordenada& coord) const;

    /**
     * @brief Calcula distância entre destino desta demanda e outra coordenada
     * @param coord Coordenada de referência
     * @return Distância euclidiana
     */
    double distanciaDestino(const Coordenada& coord) const;
};

/**
 * @brief Calcula distância euclidiana entre duas coordenadas
 * @param p1 Primeira coordenada
 * @param p2 Segunda coordenada
 * @return Distância euclidiana
 */
double calcularDistancia(const Coordenada& p1, const Coordenada& p2);

#endif // DEMANDA_HPP