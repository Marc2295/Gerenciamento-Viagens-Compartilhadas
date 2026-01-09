# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g -Iinclude

# Diretórios
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = include

# Executável
TARGET = $(BIN_DIR)/tp2.out

# Arquivos fonte
SOURCES = $(SRC_DIR)/Demanda.cpp \
          $(SRC_DIR)/Escalonador.cpp \
          $(SRC_DIR)/Corrida.cpp \
          $(SRC_DIR)/main.cpp

# Arquivos objeto
OBJECTS = $(OBJ_DIR)/Demanda.o \
          $(OBJ_DIR)/Escalonador.o \
          $(OBJ_DIR)/Corrida.o \
          $(OBJ_DIR)/main.o

# Regra padrão
all: directories $(TARGET)

# Cria diretórios se não existirem
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Compila o executável
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compilação com dependências explícitas

$(OBJ_DIR)/Demanda.o: $(SRC_DIR)/Demanda.cpp $(INC_DIR)/Demanda.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/Escalonador.o: $(SRC_DIR)/Escalonador.cpp $(INC_DIR)/Escalonador.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/Corrida.o: $(SRC_DIR)/Corrida.cpp $(INC_DIR)/Corrida.hpp $(INC_DIR)/Demanda.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(INC_DIR)/Demanda.hpp $(INC_DIR)/Escalonador.hpp $(INC_DIR)/Corrida.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	@rm -f $(OBJ_DIR)/*.o

# Limpeza completa
cleanall: clean
	@rm -f $(TARGET)
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

# Execução
run: all

.PHONY: all clean cleanall run runfile valgrind debug info help directories
