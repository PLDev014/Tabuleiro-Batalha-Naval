#include <stdio.h> //biblioteca essencial para linguagem C
#include <stdlib.h> // Para abs()

// Definições de tamanho para o tabuleiro e as matrizes de habilidade
#define BOARD_SIZE 10 // Tamanho do tabuleiro (10x10)
#define SKILL_SIZE 5  // Tamanho das matrizes de habilidade (5x5)

// --- Funções Auxiliares ---

/**
 * @brief Inicializa o tabuleiro com zeros (água).
 * @param board A matriz do tabuleiro 10x10.
 */
void initialize_Board(int board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = 0; // 0 representa água
        }
    }
}

/**
 * @brief Plota alguns navios no tabuleiro para demonstração.
 * @param board A matriz do tabuleiro 10x10.
 */
void place_Ships(int board[BOARD_SIZE][BOARD_SIZE]) {
    // Navio de 3 posições
    board[1][7] = 3;
    board[1][8] = 3;
    board[1][9] = 3;

    // Navio de 2 posições
    board[5][1] = 3;
    board[6][1] = 3;

    // Navio de 4 posições
    board[5][5] = 3;
    board[6][6] = 3;
    board[7][7] = 3;
    board[8][8] = 3;
}

/**
 * @brief Exibe o tabuleiro no console, usando caracteres diferentes para cada tipo de célula.
 * 1: Água (0)
 * 2: Navio (3)
 * 3: Área de Habilidade Afetada (5)
 * @param board A matriz do tabuleiro 10x10 a ser exibida.
 */
void print_Board(int board[BOARD_SIZE][BOARD_SIZE]) {
    printf("    ");
    for (int j = 1; j <= BOARD_SIZE; j++) {
        printf("%d ", j); // Cabeçalho das colunas
    }
    printf("\n");
    printf("  --");
    for (int j = 0; j < BOARD_SIZE; j++) {
        printf("--"); // Linha separadora
    }
    printf("\n");

    for (int i = 1; i <= BOARD_SIZE; i++) {
        if(i<BOARD_SIZE){//condição para alinhamento das colunas
        printf("%d | ", i);// Cabeçalho das linhas
        }else{
        printf("%d| ", i);// a partir daqui alinhamento das colunas ocorre
        }
        for (int j = 0; j < BOARD_SIZE; j++) {
            i--;//decrementador para atribuir valor da matriz corretamente no if dentro do loop
            if (board[i][j] == 0) {
                printf("0 "); // Água
            } else if (board[i][j] == 3) {
                printf("3 "); // Navio (Battleship)
            } else if (board[i][j] == 5) {
                printf("5 "); // Área de Habilidade Afetada
            } else {
                printf("%d ", board[i][j]); // Para qualquer outro valor (apenas para depuração, se necessário)
            }
            i++;//incrementador para não comprometer a condição do loop inicial
        }
        printf("\n");
    }
    printf("\n");
}

// --- Geração de Matrizes de Habilidade ---

/**
 * @brief Gera uma matriz representando a área de efeito em forma de cone.
 * O cone se expande da parte superior para a base.
 * @param skill_Matrix A matriz SKILL_SIZE x SKILL_SIZE para armazenar o cone.
 */
void generate_Cone_Skill(int skill_Matrix[SKILL_SIZE][SKILL_SIZE]) {
    int mid = SKILL_SIZE / 2;
    for (int i = 0; i < SKILL_SIZE; i++) {
        for (int j = 0; j < SKILL_SIZE; j++) {
            // Calcula a distância horizontal do centro para formar o cone/triângulo
            if (abs(j - mid) <= i) {
                skill_Matrix[i][j] = 1;
            } else {
                skill_Matrix[i][j] = 0;
            }
        }
    }
}

/**
 * @brief Gera uma matriz representando a área de efeito em forma de cruz.
 * O ponto de origem (centro) da cruz.
 * @param skill_Matrix A matriz SKILL_SIZE x SKILL_SIZE para armazenar a cruz.
 */
void generate_Cross_Skill(int skill_Matrix[SKILL_SIZE][SKILL_SIZE]) {
    int mid = SKILL_SIZE / 2;
    for (int i = 0; i < SKILL_SIZE; i++) {
        for (int j = 0; j < SKILL_SIZE; j++) {
            // Se a linha ou coluna for a central, marca como 1
            if (i == mid || j == mid) {
                skill_Matrix[i][j] = 1;
            } else {
                skill_Matrix[i][j] = 0;
            }
        }
    }
}

/**
 * @brief Gera uma matriz representando a área de efeito em forma de octaedro (losango).
 * O ponto de origem (centro) é o centro do losango.
 * @param skill_Matrix A matriz SKILL_SIZE x SKILL_SIZE para armazenar o octaedro.
 */
void generate_Octahedron_Skill(int skill_Matrix[SKILL_SIZE][SKILL_SIZE]) {
    int mid = SKILL_SIZE / 2;
    // O "raio" do losango é o tamanho do meio da matriz
    int radius = SKILL_SIZE / 2;
    for (int i = 0; i < SKILL_SIZE; i++) {
        for (int j = 0; j < SKILL_SIZE; j++) {
            // A soma das distâncias absolutas do centro deve ser menor ou igual ao raio
            if (abs(i - mid) + abs(j - mid) <= radius) {
                skill_Matrix[i][j] = 1;
            } else {
                skill_Matrix[i][j] = 0;
            }
        }
    }
}

// --- Aplicação de Habilidades no Tabuleiro ---

/**
 * @brief Sobrepõe uma matriz de habilidade no tabuleiro principal.
 * As posições marcadas com 1 na matriz de habilidade serão marcadas com 5 no tabuleiro.
 * @param board A matriz do tabuleiro 10x10.
 * @param skill_Matrix A matriz SKILL_SIZE x SKILL_SIZE da habilidade.
 * @param skill_Size O tamanho da matriz de habilidade.
 * @param origin_Row A linha do ponto de origem da habilidade no tabuleiro.
 * @param origin_Col A coluna do ponto de origem da habilidade no tabuleiro.
 */
void apply_Skill(int board[BOARD_SIZE][BOARD_SIZE], int skill_Matrix[SKILL_SIZE][SKILL_SIZE], int skill_Size, int origin_Row, int origin_Col) {
    // Calcula a posição de início (canto superior esquerdo) da matriz de habilidade no tabuleiro
    // para que o ponto de origem esteja centralizado.
    int start_Row = origin_Row - skill_Size / 2;
    int start_Col = origin_Col - skill_Size / 2;

    for (int i = 0; i < skill_Size; i++) {
        for (int j = 0; j < skill_Size; j++) {
            // Se a posição na matriz de habilidade é afetada (valor 1)
            if (skill_Matrix[i][j] == 1) {
                // Calcula as coordenadas correspondentes no tabuleiro principal
                int board_Row = start_Row + i;
                int board_Col = start_Col + j;

                // Verifica se as coordenadas estão dentro dos limites do tabuleiro
                if (board_Row >= 0 && board_Row < BOARD_SIZE &&
                    board_Col >= 0 && board_Col < BOARD_SIZE) {
                    // Marca a posição no tabuleiro como afetada pela habilidade (valor 5)
                    board[board_Row][board_Col] = 5;
                }
            }
        }
    }
}

// --- Função Principal ---

int main() {
    int board[BOARD_SIZE][BOARD_SIZE]; // Tabuleiro principal do jogo

    // --- Parte 1: Inicialização do Tabuleiro e Navios ---
    initialize_Board(board);
    place_Ships(board); // Adiciona alguns navios para visualização

    printf("--- Tabuleiro Inicial com Navios ---\n\n");
    print_Board(board);

    // --- Parte 2: Geração e Aplicação das Habilidades ---

    // Matrizes para as habilidades
    int cone_Skill[SKILL_SIZE][SKILL_SIZE];
    int cross_Skill[SKILL_SIZE][SKILL_SIZE];
    int octahedron_Skill[SKILL_SIZE][SKILL_SIZE];

    // Gerar as formas das habilidades
    generate_Cone_Skill(cone_Skill);
    generate_Cross_Skill(cross_Skill);
    generate_Octahedron_Skill(octahedron_Skill);

    // Definir pontos de origem para as habilidades (manual, sem input do usuário)
    // Coordenadas: (linha, coluna)
    int cone_Origin_Row = 2;
    int cone_Origin_Col = 2;

    int cross_Origin_Row = 7;
    int cross_Origin_Col = 7;

    int octahedron_Origin_Row = 4;
    int octahedron_Origin_Col = 5;
    
    // --- Exemplo de sobreposição de uma única habilidade (Cone) ---
    printf("--- Tabuleiro com Habilidade Cone (Origem: %d,%d) ---\n\n", cone_Origin_Row+1, cone_Origin_Col+1);//incrementação para ajuste na exibição correta das posições de origem
    int board_With_Cone[BOARD_SIZE][BOARD_SIZE];
    initialize_Board(board_With_Cone); // Limpa o tabuleiro temporário
    place_Ships(board_With_Cone); // Coloca navios no tabuleiro temporário
    apply_Skill(board_With_Cone, cone_Skill, SKILL_SIZE, cone_Origin_Row, cone_Origin_Col);
    print_Board(board_With_Cone);

    // --- Exemplo de sobreposição de uma única habilidade (Cruz) ---
    printf("--- Tabuleiro com Habilidade Cruz (Origem: %d,%d) ---\n\n", cross_Origin_Row+1, cross_Origin_Col+1);//incrementação para ajuste na exibição correta das posições de origem
    int board_With_Cross[BOARD_SIZE][BOARD_SIZE];
    initialize_Board(board_With_Cross); // Limpa o tabuleiro temporário
    place_Ships(board_With_Cross); // Coloca navios no tabuleiro temporário
    apply_Skill(board_With_Cross, cross_Skill, SKILL_SIZE, cross_Origin_Row, cross_Origin_Col);
    print_Board(board_With_Cross);

    // --- Exemplo de sobreposição de uma única habilidade (Octaedro) ---
    printf("--- Tabuleiro com Habilidade Octaedro (Origem: %d,%d) ---\n\n", octahedron_Origin_Row+1, octahedron_Origin_Col+1);//incrementação para ajuste na exibição correta das posições de origem
    int board_With_Octahedron[BOARD_SIZE][BOARD_SIZE];
    initialize_Board(board_With_Octahedron); // Limpa o tabuleiro temporário
    place_Ships(board_With_Octahedron); // Coloca navios no tabuleiro temporário
    apply_Skill(board_With_Octahedron, octahedron_Skill, SKILL_SIZE, octahedron_Origin_Row, octahedron_Origin_Col);
    print_Board(board_With_Octahedron);

    // --- Exemplo de sobreposição de TODAS as habilidades no mesmo tabuleiro ---
    printf("--- Tabuleiro com TODAS as Habilidades Aplicadas ---\n\n");
    initialize_Board(board); // Reinicializa o tabuleiro original
    place_Ships(board);     // Adiciona os navios novamente
    apply_Skill(board, cone_Skill, SKILL_SIZE, cone_Origin_Row, cone_Origin_Col);
    apply_Skill(board, cross_Skill, SKILL_SIZE, cross_Origin_Row, cross_Origin_Col);
    apply_Skill(board, octahedron_Skill, SKILL_SIZE, octahedron_Origin_Row, octahedron_Origin_Col);
    print_Board(board);
    /*Há alguns incrementos e decrementos utilizados ao longo do codigo foram só para fins de exibição final para o usuario.
    E não são via de regra para a escrita de um codigo    
    */
   system("pause");
    return 0;
}