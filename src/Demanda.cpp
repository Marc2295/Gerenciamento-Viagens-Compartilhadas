#include "Demanda.hpp"
#include <cmath>

// ==================== Implementação Coordenada ====================

double calcularDistancia(const Coordenada& p1, const Coordenada& p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy);
}

// ==================== Implementação Demanda ====================

Demanda::Demanda() 
    : id(-1), tempoSolicitacao(0.0), estado(DEMANDADA), corridaAssociada(-1) {
    origem = {0.0, 0.0};
    destino = {0.0, 0.0};
}

Demanda::Demanda(int id, double tempo, Coordenada orig, Coordenada dest) 
    : id(id), tempoSolicitacao(tempo), origem(orig), destino(dest), 
      estado(DEMANDADA), corridaAssociada(-1) {
}

Demanda::~Demanda() {
    // Destrutor - nenhum recurso dinâmico para liberar
}

// Getters
int Demanda::getId() const {
    return id;
}

double Demanda::getTempoSolicitacao() const {
    return tempoSolicitacao;
}

Coordenada Demanda::getOrigem() const {
    return origem;
}

Coordenada Demanda::getDestino() const {
    return destino;
}

EstadoDemanda Demanda::getEstado() const {
    return estado;
}

int Demanda::getCorridaAssociada() const {
    return corridaAssociada;
}

// Setters
void Demanda::setEstado(EstadoDemanda novoEstado) {
    estado = novoEstado;
}

void Demanda::setCorridaAssociada(int idCorrida) {
    corridaAssociada = idCorrida;
}

// Métodos de cálculo
double Demanda::calcularDistanciaViagem() const {
    return calcularDistancia(origem, destino);
}

double Demanda::distanciaOrigem(const Coordenada& coord) const {
    return calcularDistancia(origem, coord);
}

double Demanda::distanciaDestino(const Coordenada& coord) const {
    return calcularDistancia(destino, coord);
}