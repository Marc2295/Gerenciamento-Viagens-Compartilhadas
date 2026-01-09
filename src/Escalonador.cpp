#include "Escalonador.hpp"
#include <stdexcept>

// ==================== Métodos Privados ====================

int Escalonador::pai(int i) const {
    return (i - 1) / 2;
}

int Escalonador::filhoEsquerdo(int i) const {
    return 2 * i + 1;
}

int Escalonador::filhoDireito(int i) const {
    return 2 * i + 2;
}

void Escalonador::trocar(int i, int j) {
    Evento temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void Escalonador::heapifyUp(int i) {
    // Sobe o elemento até restaurar a propriedade do MinHeap
    // Complexidade: O(log n) - altura da árvore
    while (i > 0 && heap[pai(i)].tempo > heap[i].tempo) {
        trocar(i, pai(i));
        i = pai(i);
    }
}

void Escalonador::heapifyDown(int i) {
    // Desce o elemento até restaurar a propriedade do MinHeap
    // Complexidade: O(log n) - altura da árvore
    int menor = i;
    int esq = filhoEsquerdo(i);
    int dir = filhoDireito(i);

    // Verifica se filho esquerdo existe e é menor
    if (esq < tamanho && heap[esq].tempo < heap[menor].tempo) {
        menor = esq;
    }

    // Verifica se filho direito existe e é menor
    if (dir < tamanho && heap[dir].tempo < heap[menor].tempo) {
        menor = dir;
    }

    // Se o menor não é o nó atual, troca e continua descendo
    if (menor != i) {
        trocar(i, menor);
        heapifyDown(menor);
    }
}

void Escalonador::redimensionar() {
    // Dobra a capacidade do heap
    // Complexidade: O(n) - copia todos elementos
    int novaCapacidade = capacidade * 2;
    Evento* novoHeap = new Evento[novaCapacidade];

    // Copia elementos existentes
    for (int i = 0; i < tamanho; i++) {
        novoHeap[i] = heap[i];
    }

    // Libera heap antigo e atualiza
    delete[] heap;
    heap = novoHeap;
    capacidade = novaCapacidade;
}

// ==================== Métodos Públicos ====================

Escalonador::Escalonador(int capacidadeInicial) 
    : capacidade(capacidadeInicial), tamanho(0) {
    heap = new Evento[capacidade];
}

Escalonador::~Escalonador() {
    delete[] heap;
}

void Escalonador::insereEvento(const Evento& evento) {
    // Complexidade: O(log n)
    // Verifica se precisa redimensionar
    if (tamanho >= capacidade) {
        redimensionar();
    }

    // Insere no final e sobe
    heap[tamanho] = evento;
    heapifyUp(tamanho);
    tamanho++;
}

Evento Escalonador::retiraProximoEvento() {
    // Complexidade: O(log n)
    if (tamanho == 0) {
        throw std::runtime_error("Escalonador vazio");
    }

    // Salva o evento mínimo (raiz)
    Evento minimo = heap[0];

    // Move último elemento para raiz
    heap[0] = heap[tamanho - 1];
    tamanho--;

    // Desce para restaurar propriedade do heap
    if (tamanho > 0) {
        heapifyDown(0);
    }

    return minimo;
}

bool Escalonador::vazio() const {
    return tamanho == 0;
}

int Escalonador::getTamanho() const {
    return tamanho;
}

Evento Escalonador::proximoEvento() const {
    if (tamanho == 0) {
        throw std::runtime_error("Escalonador vazio");
    }
    return heap[0];
}