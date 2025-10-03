#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

// ========================================
// CONSTANTES DEL JUEGO
// ========================================
const int TABLERO_SIZE = 4;        // Tamaño del tablero
const char JUGADOR = 'X';          // Símbolo del jugador humano
const char COMPUTADORA = 'O';      // Símbolo de la computadora
const char VACIO = ' ';            // Casilla vacía

// ========================================
// ESTRUCTURA PARA REPRESENTAR UN MOVIMIENTO
// ========================================
struct Movimiento {
    int fila;
    int columna;
};

// ========================================
// CLASE PRINCIPAL DEL JUEGO
// ========================================
class JuegoGato {
private:
    vector<vector<char>> tablero;  // Tablero del juego
    
public:
    // Constructor: inicializa el tablero vacío
    JuegoGato() {
        tablero = vector<vector<char>>(TABLERO_SIZE, vector<char>(TABLERO_SIZE, VACIO));
    }
    
    // ========================================
    // FUNCIONES PARA MOSTRAR EL TABLERO
    // ========================================
    
    void mostrarTablero() {
        cout << "\n   0   1   2   3" << endl;
        cout << "  ---------------" << endl;
        
        for (int i = 0; i < TABLERO_SIZE; i++) {
            cout << i << " |";
            for (int j = 0; j < TABLERO_SIZE; j++) {
                cout << " " << tablero[i][j] << " |";
            }
            cout << endl;
            cout << "  ---------------" << endl;
        }
        cout << endl;
    }
    
    // ========================================
    // FUNCIONES PARA VERIFICAR ESTADO DEL JUEGO
    // ========================================
    
    // Verifica si un jugador ha ganado
    bool hayGanador(char jugador) {
        // Verificar filas
        for (int i = 0; i < TABLERO_SIZE; i++) {
            bool gano = true;
            for (int j = 0; j < TABLERO_SIZE; j++) {
                if (tablero[i][j] != jugador) {
                    gano = false;
                    break;
                }
            }
            if (gano) return true;
        }
        
        // Verificar columnas
        for (int j = 0; j < TABLERO_SIZE; j++) {
            bool gano = true;
            for (int i = 0; i < TABLERO_SIZE; i++) {
                if (tablero[i][j] != jugador) {
                    gano = false;
                    break;
                }
            }
            if (gano) return true;
        }
        
        // Verificar diagonal principal (arriba-izquierda a abajo-derecha)
        bool ganoDiagonal1 = true;
        for (int i = 0; i < TABLERO_SIZE; i++) {
            if (tablero[i][i] != jugador) {
                ganoDiagonal1 = false;
                break;
            }
        }
        if (ganoDiagonal1) return true;
        
        // Verificar diagonal secundaria (arriba-derecha a abajo-izquierda)
        bool ganoDiagonal2 = true;
        for (int i = 0; i < TABLERO_SIZE; i++) {
            if (tablero[i][TABLERO_SIZE - 1 - i] != jugador) {
                ganoDiagonal2 = false;
                break;
            }
        }
        if (ganoDiagonal2) return true;
        
        return false;
    }
    
    // Verifica si el tablero está lleno (empate)
    bool tableroLleno() {
        for (int i = 0; i < TABLERO_SIZE; i++) {
            for (int j = 0; j < TABLERO_SIZE; j++) {
                if (tablero[i][j] == VACIO) {
                    return false;
                }
            }
        }
        return true;
    }
    
    // Obtiene todos los movimientos disponibles
    vector<Movimiento> obtenerMovimientosDisponibles() {
        vector<Movimiento> movimientos;
        for (int i = 0; i < TABLERO_SIZE; i++) {
            for (int j = 0; j < TABLERO_SIZE; j++) {
                if (tablero[i][j] == VACIO) {
                    movimientos.push_back({i, j});
                }
            }
        }
        return movimientos;
    }
    
    // ========================================
    // FUNCIONES DE EVALUACIÓN PARA MINIMAX
    // ========================================
    
    // Evalúa el estado actual del tablero
    // Retorna: +10 si gana computadora, -10 si gana jugador, 0 en otro caso
    int evaluarTablero() {
        if (hayGanador(COMPUTADORA)) {
            return 10;
        } else if (hayGanador(JUGADOR)) {
            return -10;
        }
        return 0;
    }
    
    // ========================================
    // ALGORITMO MINIMAX CON PODA ALFA-BETA
    // ========================================
    
    int minimax(int profundidad, bool esMaximizando, int alfa, int beta) {
        // Calcular el puntaje del estado actual
        int puntaje = evaluarTablero();
        
        // Caso base: si hay un ganador, retornar el puntaje
        if (puntaje == 10) {
            return puntaje - profundidad;  // Penalizar movimientos más largos
        }
        if (puntaje == -10) {
            return puntaje + profundidad;  // Penalizar movimientos más largos
        }
        
        // Si el tablero está lleno, es empate
        if (tableroLleno()) {
            return 0;
        }
        
        // Limitar profundidad para optimizar rendimiento
        if (profundidad >= 6) {
            return 0;
        }
        
        // Turno de maximizar (Computadora - O)
        if (esMaximizando) {
            int mejorPuntaje = numeric_limits<int>::min();
            
            // Probar todos los movimientos posibles
            vector<Movimiento> movimientos = obtenerMovimientosDisponibles();
            for (const Movimiento& mov : movimientos) {
                // Hacer el movimiento
                tablero[mov.fila][mov.columna] = COMPUTADORA;
                
                // Llamada recursiva
                int puntaje = minimax(profundidad + 1, false, alfa, beta);
                
                // Deshacer el movimiento
                tablero[mov.fila][mov.columna] = VACIO;
                
                // Actualizar mejor puntaje y alfa
                mejorPuntaje = max(mejorPuntaje, puntaje);
                alfa = max(alfa, mejorPuntaje);
                
                // Poda Beta: si beta <= alfa, no seguir explorando
                if (beta <= alfa) {
                    break;
                }
            }
            return mejorPuntaje;
        }
        // Turno de minimizar (Jugador - X)
        else {
            int mejorPuntaje = numeric_limits<int>::max();
            
            // Probar todos los movimientos posibles
            vector<Movimiento> movimientos = obtenerMovimientosDisponibles();
            for (const Movimiento& mov : movimientos) {
                // Hacer el movimiento
                tablero[mov.fila][mov.columna] = JUGADOR;
                
                // Llamada recursiva
                int puntaje = minimax(profundidad + 1, true, alfa, beta);
                
                // Deshacer el movimiento
                tablero[mov.fila][mov.columna] = VACIO;
                
                // Actualizar mejor puntaje y beta
                mejorPuntaje = min(mejorPuntaje, puntaje);
                beta = min(beta, mejorPuntaje);
                
                // Poda Alfa: si beta <= alfa, no seguir explorando
                if (beta <= alfa) {
                    break;
                }
            }
            return mejorPuntaje;
        }
    }
    
    // Encuentra el mejor movimiento para la computadora
    Movimiento encontrarMejorMovimiento() {
        int mejorPuntaje = numeric_limits<int>::min();
        Movimiento mejorMov = {-1, -1};
        
        // Evaluar todos los movimientos posibles
        vector<Movimiento> movimientos = obtenerMovimientosDisponibles();
        for (const Movimiento& mov : movimientos) {
            // Hacer el movimiento temporalmente
            tablero[mov.fila][mov.columna] = COMPUTADORA;
            
            // Calcular el puntaje de este movimiento
            int puntaje = minimax(0, false, numeric_limits<int>::min(), numeric_limits<int>::max());
            
            // Deshacer el movimiento
            tablero[mov.fila][mov.columna] = VACIO;
            
            // Si este movimiento es mejor, guardarlo
            if (puntaje > mejorPuntaje) {
                mejorPuntaje = puntaje;
                mejorMov = mov;
            }
        }
        
        return mejorMov;
    }
    
    // ========================================
    // FUNCIONES PARA REALIZAR MOVIMIENTOS
    // ========================================
    
    // Turno del jugador humano
    bool turnoJugador() {
        int fila, columna;
        
        while (true) {
            cout << "Tu turno (X)" << endl;
            cout << "Ingresa la fila (0-3): ";
            cin >> fila;
            
            // Verificar si la entrada es válida
            if (cin.fail()) {
                cin.clear();  // Limpiar el error
                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignorar entrada incorrecta
                cout << "Error: Por favor ingresa un número válido." << endl;
                continue;
            }
            
            cout << "Ingresa la columna (0-3): ";
            cin >> columna;
            
            // Verificar si la entrada es válida
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Error: Por favor ingresa un número válido." << endl;
                continue;
            }
            
            // Validar que las coordenadas estén en el rango correcto
            if (fila < 0 || fila >= TABLERO_SIZE || columna < 0 || columna >= TABLERO_SIZE) {
                cout << "Error: Las coordenadas deben estar entre 0 y 3." << endl;
                continue;
            }
            
            // Validar que la casilla esté vacía
            if (tablero[fila][columna] != VACIO) {
                cout << "Error: Esa casilla ya está ocupada. Intenta otra." << endl;
                continue;
            }
            
            // Realizar el movimiento
            tablero[fila][columna] = JUGADOR;
            break;
        }
        
        return true;
    }
    
    // Turno de la computadora
    void turnoComputadora() {
        cout << "Turno de la computadora (O)..." << endl;
        
        Movimiento mejorMov = encontrarMejorMovimiento();
        
        if (mejorMov.fila != -1 && mejorMov.columna != -1) {
            tablero[mejorMov.fila][mejorMov.columna] = COMPUTADORA;
            cout << "La computadora jugó en: (" << mejorMov.fila << ", " << mejorMov.columna << ")" << endl;
        }
    }
    
    // ========================================
    // FUNCIÓN PRINCIPAL DEL JUEGO
    // ========================================
    
    void jugar() {
        cout << "========================================" << endl;
        cout << "       JUEGO DE GATO 4x4" << endl;
        cout << "   Jugador: X | Computadora: O" << endl;
        cout << "========================================" << endl;
        
        char turnoActual;
        char opcion;
        
        // Preguntar quién inicia
        while (true) {
            cout << "\n¿Quién inicia? (1: Tú, 2: Computadora): ";
            cin >> opcion;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Por favor, ingresa 1 o 2." << endl;
                continue;
            }
            
            if (opcion == '1') {
                turnoActual = JUGADOR;
                break;
            } else if (opcion == '2') {
                turnoActual = COMPUTADORA;
                break;
            } else {
                cout << "Por favor, ingresa 1 o 2." << endl;
            }
        }
        
        // Bucle principal del juego
        while (true) {
            mostrarTablero();
            
            // Verificar si el juego terminó
            if (hayGanador(JUGADOR)) {
                cout << "¡Felicidades! ¡Ganaste!" << endl;
                break;
            } else if (hayGanador(COMPUTADORA)) {
                cout << "La computadora ganó. ¡Intenta de nuevo!" << endl;
                break;
            } else if (tableroLleno()) {
                cout << "¡Empate! Buen juego." << endl;
                break;
            }
            
            // Realizar el turno correspondiente
            if (turnoActual == JUGADOR) {
                turnoJugador();
                turnoActual = COMPUTADORA;
            } else {
                turnoComputadora();
                turnoActual = JUGADOR;
            }
        }
        
        // Mostrar tablero final
        mostrarTablero();
        
        // Preguntar si quiere jugar de nuevo
        while (true) {
            cout << "\n¿Quieres jugar de nuevo? (s/n): ";
            cin >> opcion;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            
            if (opcion == 's' || opcion == 'S') {
                // Reiniciar el tablero
                tablero = vector<vector<char>>(TABLERO_SIZE, vector<char>(TABLERO_SIZE, VACIO));
                jugar();
                break;
            } else if (opcion == 'n' || opcion == 'N') {
                cout << "\n¡Gracias por jugar! 👋" << endl;
                break;
            } else {
                cout << "Por favor, ingresa 's' o 'n'." << endl;
            }
        }
    }
};

// ========================================
// FUNCIÓN MAIN
// ========================================

int main() {
    JuegoGato juego;
    juego.jugar();
    
    return 0;
}
