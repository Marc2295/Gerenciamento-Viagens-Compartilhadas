#ifndef CORRIDA_HPP
#define CORRIDA_HPP

#include "Demanda.hpp"

/**
 * @file Corrida.hpp
 * @brief Estrutura para gerenciar corridas compartilhadas
 * 
 * Armazena informações sobre uma corrida que pode atender
 * uma ou mais demandas compartilhadas
 */

struct InfoCorrida {
    int* demandasIds;       // IDs das demandas nesta corrida
    int numDemandas;        // Número de demandas
    int capacidade;         // Capacidade alocada
    double distanciaTotal;  // Distância total da corrida
    double tempoInicio;     // Tempo de início (primeira coleta)
    bool processada;        // Se já foi escalonada

    InfoCorrida();
    ~InfoCorrida();

    /**
     * @brief Adiciona demanda à corrida
     * @param idDemanda ID da demanda
     */
    void adicionarDemanda(int idDemanda);

    /**
     * @brief Calcula distância total da rota
     * @param demandas Array de demandas
     * @param gama Velocidade do veículo
     */
    void calcularRota(Demanda* demandas, double gama);

    /**
     * @brief Calcula eficiência da corrida compartilhada
     * @param demandas Array de demandas
     * @return Eficiência em porcentagem
     */
    double calcularEficiencia(Demanda* demandas) const;
};

#endif // CORRIDA_HPP