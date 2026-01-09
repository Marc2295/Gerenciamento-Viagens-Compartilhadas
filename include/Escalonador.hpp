#ifndef ESCALONADOR_HPP
#define ESCALONADOR_HPP

/**
 * @file Escalonador.hpp
 * @brief TAD Escalonador - Fila de prioridade para simulação de eventos discretos
 * 
 * Implementado como MinHeap baseado em tempo
 * Complexidade: O(log n) para inserção e remoção
 */

enum TipoEvento {
    COLETA,       // Embarque de passageiro
    ENTREGA       // Desembarque de passageiro
};

struct Evento {
    double tempo;           // Momento em que o evento ocorre
    TipoEvento tipo;        // Tipo do evento
    int idCorrida;          // ID da corrida associada
    int idDemanda;          // ID da demanda associada
    int indiceTrecho;       // Índice do trecho na corrida
};

class Escalonador {
private:
    Evento* heap;           // Array para armazenar eventos
    int capacidade;         // Capacidade máxima do heap
    int tamanho;            // Número atual de elementos

    /**
     * @brief Retorna índice do pai
     * @param i Índice do nó
     * @return Índice do pai
     */
    int pai(int i) const;

    /**
     * @brief Retorna índice do filho esquerdo
     * @param i Índice do nó
     * @return Índice do filho esquerdo
     */
    int filhoEsquerdo(int i) const;

    /**
     * @brief Retorna índice do filho direito
     * @param i Índice do nó
     * @return Índice do filho direito
     */
    int filhoDireito(int i) const;

    /**
     * @brief Sobe elemento para manter propriedade do heap
     * @param i Índice do elemento
     * Complexidade: O(log n)
     */
    void heapifyUp(int i);

    /**
     * @brief Desce elemento para manter propriedade do heap
     * @param i Índice do elemento
     * Complexidade: O(log n)
     */
    void heapifyDown(int i);

    /**
     * @brief Troca dois elementos no heap
     * @param i Índice do primeiro elemento
     * @param j Índice do segundo elemento
     */
    void trocar(int i, int j);

    /**
     * @brief Redimensiona o heap quando necessário
     * Complexidade: O(n)
     */
    void redimensionar();

public:
    /**
     * @brief Construtor - inicializa heap vazio
     * @param capacidadeInicial Capacidade inicial do heap (padrão: 100)
     * Complexidade: O(1)
     */
    Escalonador(int capacidadeInicial = 100);

    /**
     * @brief Destrutor - libera memória alocada
     * Complexidade: O(1)
     */
    ~Escalonador();

    /**
     * @brief Insere um evento no escalonador
     * @param evento Evento a ser inserido
     * Complexidade: O(log n)
     */
    void insereEvento(const Evento& evento);

    /**
     * @brief Remove e retorna o próximo evento (menor tempo)
     * @return Evento com menor tempo
     * Complexidade: O(log n)
     */
    Evento retiraProximoEvento();

    /**
     * @brief Verifica se o escalonador está vazio
     * @return true se vazio, false caso contrário
     * Complexidade: O(1)
     */
    bool vazio() const;

    /**
     * @brief Retorna número de eventos no escalonador
     * @return Número de eventos
     * Complexidade: O(1)
     */
    int getTamanho() const;

    /**
     * @brief Retorna o próximo evento sem removê-lo
     * @return Evento com menor tempo
     * Complexidade: O(1)
     */
    Evento proximoEvento() const;
};

#endif // ESCALONADOR_HPP