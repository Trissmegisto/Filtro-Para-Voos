# Compilador e flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -I include

# Diretórios
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Nome do executável
TARGET = $(BIN_DIR)/tp3.out

# Regra principal
all: directories $(TARGET)

# Cria diretórios necessários
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Regra para o executável
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Regra para os arquivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpa arquivos gerados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Regra para executar o programa
run: all
	./$(TARGET)

# Phony targets
.PHONY: all clean run directories