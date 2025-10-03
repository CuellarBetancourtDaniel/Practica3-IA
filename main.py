import math

# ========================================
# CONSTANTES DEL JUEGO
# ========================================
TABLERO_SIZE = 4  # Tamaño del tablero
JUGADOR = 'X'     # Símbolo del jugador humano
COMPUTADORA = 'O' # Símbolo de la computadora
VACIO = ' '       # Casilla vacía

# ========================================
# FUNCIONES PARA EL TABLERO
# ========================================

def crear_tablero():
    """Crea un tablero vacío de 4x4"""
    return [[VACIO for _ in range(TABLERO_SIZE)] for _ in range(TABLERO_SIZE)]

def mostrar_tablero(tablero):
    """Muestra el tablero en consola de forma visual"""
    print("\n   0   1   2   3")
    print("  ---------------")
    for i in range(TABLERO_SIZE):
        fila = f"{i} |"
        for j in range(TABLERO_SIZE):
            fila += f" {tablero[i][j]} |"
        print(fila)
        print("  ---------------")
    print()

def hay_ganador(tablero, jugador):
    """
    Verifica si un jugador ha ganado
    Revisa filas, columnas y diagonales
    """
    # Verificar filas
    for fila in tablero:
        if all(celda == jugador for celda in fila):
            return True
    
    # Verificar columnas
    for col in range(TABLERO_SIZE):
        if all(tablero[fila][col] == jugador for fila in range(TABLERO_SIZE)):
            return True
    
    # Verificar diagonal principal (de arriba-izquierda a abajo-derecha)
    if all(tablero[i][i] == jugador for i in range(TABLERO_SIZE)):
        return True
    
    # Verificar diagonal secundaria (de arriba-derecha a abajo-izquierda)
    if all(tablero[i][TABLERO_SIZE - 1 - i] == jugador for i in range(TABLERO_SIZE)):
        return True
    
    return False

def tablero_lleno(tablero):
    """Verifica si el tablero está completamente lleno (empate)"""
    return all(tablero[i][j] != VACIO for i in range(TABLERO_SIZE) for j in range(TABLERO_SIZE))

def obtener_movimientos_disponibles(tablero):
    """Retorna una lista de todas las posiciones vacías disponibles"""
    movimientos = []
    for i in range(TABLERO_SIZE):
        for j in range(TABLERO_SIZE):
            if tablero[i][j] == VACIO:
                movimientos.append((i, j))
    return movimientos

# ========================================
# ALGORITMO MINIMAX CON PODA ALFA-BETA
# ========================================

def evaluar_tablero(tablero):
    """
    Evalúa el estado del tablero y retorna un puntaje:
    +10 si gana la computadora
    -10 si gana el jugador
    0 si hay empate o juego en curso
    """
    if hay_ganador(tablero, COMPUTADORA):
        return 10
    elif hay_ganador(tablero, JUGADOR):
        return -10
    else:
        return 0

def minimax(tablero, profundidad, es_maximizando, alfa, beta):
    """
    Implementación del algoritmo Minimax con poda Alfa-Beta
    
    Parámetros:
    - tablero: estado actual del juego
    - profundidad: nivel de profundidad en el árbol de decisión
    - es_maximizando: True si es turno de maximizar (computadora), False si es minimizar (jugador)
    - alfa: mejor valor para el maximizador
    - beta: mejor valor para el minimizador
    
    Retorna el mejor puntaje posible para el jugador actual
    """
    # Calcular el puntaje del estado actual
    puntaje = evaluar_tablero(tablero)
    
    # Caso base: si hay un ganador o empate, retornar el puntaje
    if puntaje == 10:  # Gana computadora
        return puntaje - profundidad  # Penalizar movimientos más largos
    if puntaje == -10:  # Gana jugador
        return puntaje + profundidad  # Penalizar movimientos más largos
    if tablero_lleno(tablero):  # Empate
        return 0
    
    # Limitar profundidad para que sea más rápido (opcional)
    if profundidad >= 6:  # Limitar a 6 niveles de profundidad
        return 0
    
    # Turno de maximizar (Computadora - O)
    if es_maximizando:
        mejor_puntaje = -math.inf
        
        # Probar todos los movimientos posibles
        for (i, j) in obtener_movimientos_disponibles(tablero):
            # Hacer el movimiento
            tablero[i][j] = COMPUTADORA
            
            # Llamada recursiva para el siguiente nivel
            puntaje = minimax(tablero, profundidad + 1, False, alfa, beta)
            
            # Deshacer el movimiento
            tablero[i][j] = VACIO
            
            # Actualizar mejor puntaje y alfa
            mejor_puntaje = max(mejor_puntaje, puntaje)
            alfa = max(alfa, mejor_puntaje)
            
            # Poda Beta: si beta <= alfa, no necesitamos seguir explorando
            if beta <= alfa:
                break
        
        return mejor_puntaje
    
    # Turno de minimizar (Jugador - X)
    else:
        mejor_puntaje = math.inf
        
        # Probar todos los movimientos posibles
        for (i, j) in obtener_movimientos_disponibles(tablero):
            # Hacer el movimiento
            tablero[i][j] = JUGADOR
            
            # Llamada recursiva para el siguiente nivel
            puntaje = minimax(tablero, profundidad + 1, True, alfa, beta)
            
            # Deshacer el movimiento
            tablero[i][j] = VACIO
            
            # Actualizar mejor puntaje y beta
            mejor_puntaje = min(mejor_puntaje, puntaje)
            beta = min(beta, mejor_puntaje)
            
            # Poda Alfa: si beta <= alfa, no necesitamos seguir explorando
            if beta <= alfa:
                break
        
        return mejor_puntaje

def mejor_movimiento(tablero):
    """
    Encuentra el mejor movimiento para la computadora
    usando el algoritmo Minimax con poda Alfa-Beta
    """
    mejor_puntaje = -math.inf
    mejor_mov = None
    
    # Evaluar todos los movimientos posibles
    for (i, j) in obtener_movimientos_disponibles(tablero):
        # Hacer el movimiento temporalmente
        tablero[i][j] = COMPUTADORA
        
        # Calcular el puntaje de este movimiento
        puntaje = minimax(tablero, 0, False, -math.inf, math.inf)
        
        # Deshacer el movimiento
        tablero[i][j] = VACIO
        
        # Si este movimiento es mejor, guardarlo
        if puntaje > mejor_puntaje:
            mejor_puntaje = puntaje
            mejor_mov = (i, j)
    
    return mejor_mov

# ========================================
# LÓGICA DEL JUEGO
# ========================================

def turno_jugador(tablero):
    """Maneja el turno del jugador humano"""
    while True:
        try:
            print("Tu turno (X)")
            fila = int(input("Ingresa la fila (0-3): "))
            columna = int(input("Ingresa la columna (0-3): "))
            
            # Validar que la entrada esté en el rango correcto
            if fila < 0 or fila >= TABLERO_SIZE or columna < 0 or columna >= TABLERO_SIZE:
                print("Error: Las coordenadas deben estar entre 0 y 3.")
                continue
            
            # Validar que la casilla esté vacía
            if tablero[fila][columna] != VACIO:
                print("Error: Esa casilla ya está ocupada. Intenta otra.")
                continue
            
            # Realizar el movimiento
            tablero[fila][columna] = JUGADOR
            break
            
        except ValueError:
            print("Error: Por favor ingresa números válidos.")
        except Exception as e:
            print(f"Error: {e}")

def turno_computadora(tablero):
    """Maneja el turno de la computadora usando Minimax"""
    print("Turno de la computadora (O)...")
    movimiento = mejor_movimiento(tablero)
    
    if movimiento:
        fila, columna = movimiento
        tablero[fila][columna] = COMPUTADORA
        print(f"La computadora jugó en: ({fila}, {columna})")

def jugar():
    """Función principal del juego"""
    print("=" * 40)
    print("   JUEGO DE GATO 4x4")
    print("   Jugador: X | Computadora: O")
    print("=" * 40)
    
    tablero = crear_tablero()
    
    # Preguntar quién inicia
    while True:
        inicio = input("\n¿Quién inicia? (1: Tú, 2: Computadora): ")
        if inicio in ['1', '2']:
            turno_actual = JUGADOR if inicio == '1' else COMPUTADORA
            break
        print("Por favor, ingresa 1 o 2.")
    
    # Bucle principal del juego
    while True:
        mostrar_tablero(tablero)
        
        # Verificar si el juego terminó
        if hay_ganador(tablero, JUGADOR):
            print("¡Felicidades! ¡Ganaste!")
            break
        elif hay_ganador(tablero, COMPUTADORA):
            print("La computadora ganó. ¡Intenta de nuevo!")
            break
        elif tablero_lleno(tablero):
            print(" ¡Empate! Buen juego.")
            break
        
        # Realizar el turno correspondiente
        if turno_actual == JUGADOR:
            turno_jugador(tablero)
            turno_actual = COMPUTADORA
        else:
            turno_computadora(tablero)
            turno_actual = JUGADOR
    
    # Mostrar tablero final
    mostrar_tablero(tablero)
    
    # Preguntar si quiere jugar de nuevo
    while True:
        respuesta = input("\n¿Quieres jugar de nuevo? (s/n): ").lower()
        if respuesta == 's':
            jugar()
            break
        elif respuesta == 'n':
            print("\n¡Gracias por jugar! 👋")
            break
        else:
            print("Por favor, ingresa 's' o 'n'.")

# ========================================
# INICIAR EL JUEGO
# ========================================

if __name__ == "__main__":
    jugar()
