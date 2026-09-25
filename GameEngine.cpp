#include "GameEngine.h"

/*
Permite controlar la entrada de las teclas para que handlePlayInput, handlePauseInput y handleReplayInput puedan funcionar bien
utilizando componentes de SFML.
*/
void GameEngine::processInput() {
	while (const std::optional<sf::Event> event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
		if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
			if (currentState == GameState::PLAYING) {
				handlePlayInput(keyPress->code);
			} else if(currentState == GameState::PAUSE) {
				handlePausedInput(keyPress->code);
			} else if(currentState == GameState::GAME_OVER) {
				handleReplayInput(keyPress->code);
			}
		}
	}
}

/*
Este método permite manejar el control del tiempo para incluir eventos, evaluar posiciones correctas en el tablero, llamar a los generadores de piezas,
etc. Básicamente es un sistema de control que se llama dentro de run().
*/
void GameEngine::update(float deltaTime) {
	if (currentState == GameState::GAME_OVER) {
		if (isAutoReplay) {
			replayTimer += deltaTime;
			if (replayTimer >= 0.15f) { 
				replayTimer = 0.0f;
				MovePiece outMove;
				if (movList.getNextMoveStep(outMove)) {
					currentPieceType = outMove.pieceType;
					currentX = outMove.targetX;
					currentY = outMove.targetY;
					currentRotation = outMove.rotation;
				} else {
					isAutoReplay = false; 
				}
			}
		}
		return; 
	}
	
	if(currentState != GameState::PLAYING) return;
	gameTime -= deltaTime;
	
	EventData currentEvent;
	while (eventQueue.showNextEvent(currentEvent) && gameTime <= currentEvent.triggerTime) {
		if (eventQueue.popFront(currentEvent)) {
			std::cout << "Se quito un evento de la cola..." << std::endl;
			if (currentEvent.eventType == 1) {
				gameTime += currentEvent.parameter; 
			}
			
			//Colocar el resto de eventos por aquí
		}
	}
	
	if(isTimeExpired()){
		gameTime = 0.0f;
		currentState = GameState::GAME_OVER;
		scoreManager.registerNewScore("Jugador", score);
		std::cout << "TIEMPO EXPIRADO..."<<std::endl;
		return;
	}
	
	dropTimer += deltaTime;
	if(dropTimer >= dropInterval) {
		dropTimer = 0.0f;
		if(isValidPosition(currentPieceType, currentRotation, currentX, currentY + 1)) {
			currentY++;
		} else {
			std::cout << "1. Fijando pieza..." << std::endl;
			lockPiece(); 
			
			std::cout << "2. Limpiando lineas..." << std::endl;
			int linesCleared = board.clearCompleteRows(); 
			if(linesCleared > 0){
				score += linesCleared * 100 + (linesCleared > 1 ? (linesCleared - 1) * 100 : 0);
				if(linesCleared >= 2){
					timeBonusEvent(linesCleared);
				}
			}
			
			std::cout << "3. Generando nueva pieza..." << std::endl;
			spawnNewPiece();
			
			std::cout << "4. Pieza generada con exito." << std::endl;
			
			if(!isValidPosition(currentPieceType, currentRotation, currentX, currentY)){
				currentState = GameState::GAME_OVER;
				scoreManager.addScore("Jugador", score);
				scoreManager.sortByQuickSort();
				scoreManager.saveFile("scores.txt");
				return;
			}
		}
	}
}
/*
Esta función dibuja la cola de piezas que vienen
*/
void GameEngine::renderNextPieces(sf::RectangleShape& cellShape){
	const float TILE_SIZE = 30.0f;
	const float SCALE = 0.75f; 
	const float PANEL_OFFSET_X = 470.0f; 
	const float PANEL_START_Y = 50.0f;
	const float ITEM_SPACING = 90.0f;     
	
	cellShape.setSize(sf::Vector2f(TILE_SIZE * SCALE, TILE_SIZE * SCALE)); 
	cellShape.setFillColor(sf::Color::White);                               
	cellShape.setOutlineThickness(-1.0f);                                  
	cellShape.setOutlineColor(sf::Color::Black);                          
	
	int next0 = piece.getPieceAt(0);
	int next1 = piece.getPieceAt(1);
	int next2 = piece.getPieceAt(2);
	
	//Dibuja la primera pieza en la fila
	if (next0 >= 0) {
		float startY = PANEL_START_Y;
		for (int r = 0; r < 4; ++r) {
			for (int c = 0; c < 4; ++c) {
				if (PIECE_SHAPES[next0][0][r][c] != 0) {
					cellShape.setPosition(sf::Vector2f(PANEL_OFFSET_X + (c * (TILE_SIZE * SCALE)), startY + (r * (TILE_SIZE * SCALE))));
					window.draw(cellShape);
				}
			}
		}
	}
	
	//Dibuja la segunda pieza en la fila
	if (next1 >= 0) {
		float startY = PANEL_START_Y + ITEM_SPACING;
		for (int r = 0; r < 4; ++r) {
			for (int c = 0; c < 4; ++c) {
				if (PIECE_SHAPES[next1][0][r][c] != 0) {
					cellShape.setPosition(sf::Vector2f(PANEL_OFFSET_X + (c * (TILE_SIZE * SCALE)), startY + (r * (TILE_SIZE * SCALE))));
					window.draw(cellShape);
				}
			}
		}
	}
	
	//Dibuja la tercera pieza en la fila
	if (next2 >= 0) {
		float startY = PANEL_START_Y + (ITEM_SPACING * 2);
		for (int r = 0; r < 4; ++r) {
			for (int c = 0; c < 4; ++c) {
				if (PIECE_SHAPES[next2][0][r][c] != 0) {
					cellShape.setPosition(sf::Vector2f(PANEL_OFFSET_X + (c * (TILE_SIZE * SCALE)), startY + (r * (TILE_SIZE * SCALE))));
					window.draw(cellShape);
				}
			}
		}
	}
}

/*
Esta función dibuja la pieza que se ha cambiado y la mantien posicionada al lado izquierdo del tablero.
Esa pieza se puede usar como una especie de comodín para usarse más adelante.
Cada que se decida utilizar esa pieza esta se va a intercambiar con la pieza que actualmente esté cayendo en el tablero.
*/
void GameEngine::renderHoldPiece(sf::RectangleShape& cellShape){
	int holdPieceType = holdStack.peek();
	if (holdPieceType < 0) return;
	
	const float TILE_SIZE = 30.0f;
	const float SCALE = 0.75f;
	const float HOLD_PANEL_X = 20.0f; 
	const float HOLD_PANEL_Y = 60.0f;
	
	cellShape.setSize(sf::Vector2f(TILE_SIZE * SCALE, TILE_SIZE * SCALE));
	cellShape.setOutlineThickness(-1.0f);
	cellShape.setOutlineColor(sf::Color::Black);
	cellShape.setFillColor(sf::Color(255, 215, 0)); 
	
	for (int r = 0; r < 4; ++r) {
		for (int c = 0; c < 4; ++c) {
			if (PIECE_SHAPES[holdPieceType][0][r][c] != 0) {
				cellShape.setPosition(sf::Vector2f(
												   HOLD_PANEL_X + (c * (TILE_SIZE * SCALE)),
												   HOLD_PANEL_Y + (r * (TILE_SIZE * SCALE))
												   ));
				window.draw(cellShape);
			}
		}
	}
}
//Rendirza el puntaje 
void GameEngine::renderScore(){
	if (!fontLoaded) return;
	
	
	sf::Text scoreText(font, "SCORE\n" + std::to_string(score), 18);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition({20.0f, 200.0f});
	
	window.draw(scoreText);
}

/*
Permite que las piezas creadas sean visibles, así como su animación de eliminación de filas y caída.
*/
void GameEngine::render() {
	window.clear(sf::Color::Black);
	
	if(currentState == GameState::PLAYING || currentState == GameState::PAUSE || currentState == GameState::GAME_OVER) {
		const float TILE_SIZE = 30.0f;
		const float BOARD_OFFSET_X = 150.0f;
		sf::RectangleShape cellShape(sf::Vector2f(TILE_SIZE - 1.0f, TILE_SIZE - 1.0f));
		
		//Dibuja el tablero
		for(int r = 0; r < 20; r++) {
			for(int c = 0; c < 10; c++) {
				float drawX = BOARD_OFFSET_X + (c * TILE_SIZE);
				float drawY = r * TILE_SIZE;
				cellShape.setPosition(sf::Vector2f(drawX, drawY));
				if(board.isCellOccupied(r, c)) {
					cellShape.setFillColor(sf::Color::Cyan); 
				} else {
					cellShape.setFillColor(sf::Color(30, 30, 30)); 
				}
				window.draw(cellShape);
			}
		}
		
		//Dibuja la pieza actual o la del replay temporal
		if(!piece.isEmpty()) {
			cellShape.setFillColor(sf::Color::Red);
			for(int r = 0; r < 4; r++) {
				for(int c = 0; c < 4; c++) {
					if(PIECE_SHAPES[currentPieceType][currentRotation][r][c] != 0) {
						int targetX = currentX + c;
						int targetY = currentY + r;
						if(targetY >= 0 && targetY < 20 && targetX >= 0 && targetX < 10) {
							float drawX = BOARD_OFFSET_X + (targetX * TILE_SIZE);
							float drawY = targetY * TILE_SIZE;
							cellShape.setPosition(sf::Vector2f(drawX, drawY));
							window.draw(cellShape);
						}
					}
				}
			}
		}
		
		renderNextPieces(cellShape);
		renderHoldPiece(cellShape);
		renderScore();
		
		//Muestra la pantalla y los controles del replay
		if (currentState == GameState::GAME_OVER) {
			
			sf::RectangleShape overlay(sf::Vector2f(10 * TILE_SIZE, 20 * TILE_SIZE));
			overlay.setPosition(sf::Vector2f(BOARD_OFFSET_X, 0.0f)); 
			overlay.setFillColor(sf::Color(0, 0, 0, 180)); 
			window.draw(overlay);
			
			if (fontLoaded) {
				sf::Text gameOverText(font, "GAME OVER", 35);
				gameOverText.setFillColor(sf::Color::Red);
				gameOverText.setPosition(sf::Vector2f(BOARD_OFFSET_X + 15.0f, 150.0f)); 
				window.draw(gameOverText);
				
				sf::Text replayText(font, "MODO REPLAY\n\n[ <- ] Retroceder\n[ -> ] Avanzar\n[ R ] Repetir Todo", 18);
				replayText.setFillColor(sf::Color::Yellow);
				replayText.setPosition(sf::Vector2f(BOARD_OFFSET_X + 20.0f, 250.0f)); 
				window.draw(replayText);
			}
		}
	}
	window.display();
}

/*
Valida si la posición en donde se va a colocar la pieza es válida en base a las dimensiones de cada pieza, de modo conque una 
sola parte de una pieza colisione, no se pueda agregar.
*/
bool GameEngine::isValidPosition(int pieceType, int rotation, int newX, int newY) const {
	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 4; col++){
			if(PIECE_SHAPES[pieceType][rotation][row][col] != 0){
				int targetX = newX + col;
				int targetY = newY + row;
				
				if(targetX < 0 || targetX >= 10 || targetY >= 20){
					return false;
				}
				if(targetY < 0) continue;
				if(board.isCellOccupied(targetY,targetX)){
					return false;
				}
			}
		}
	}
	return true;
}

/*
Posiciona una pieza en cuanto colosiona con otra o bien toca el fondo del tablero.
*/
void GameEngine::lockPiece() {
	for(int row = 0; row < 4; row++){
		for(int col = 0; col < 4; col++){
			if(PIECE_SHAPES[currentPieceType][currentRotation][row][col] != 0){
				int boardX = currentX + col;
				int boardY = currentY + row;
				if(boardX >= 0 && boardX < 10 && boardY >= 0 && boardY < 20){
					board.setCell(boardY, boardX, currentPieceType + 1);
				}
				
			}
		}
	}
}
/*
Extrae y controla las piezas generadas por la clase Piece.
*/
void GameEngine::spawnNewPiece() {
	holdStack.setUsedInTurn(false);
	currentPieceType = piece.deletePiece();
	int remainingInBag = piece.getPieceSize();
	currentX = 3;
	currentY = 0;
	currentRotation = 0;
}

//Controla el tiempo de partida en el juego.
bool GameEngine::isTimeExpired() const{
	return gameTime <= 0.0f;
}

/*
Este método maneja el evento de bonus de tiempo, la mecánica de este funciona rompiendo dos o más filas consecutivas.
2 filas: +5s y 5 segundos para activar el evento de la cola a la partida
3 filas: +7.5s y 10 segundos para activar el evento de la cola a la partida
4 filas: +10s y 15 segundos para activar el evento de la cola a la partida
*/
bool GameEngine::timeBonusEvent(int rowsCleared){
	float bonusTime = 0.0f;
	bool appliedBonus = false;
	switch(rowsCleared){
	case 2:
		eventQueue.insertSorted({1, gameTime - 5.0f, 5.0f});
		appliedBonus = true;
		std::cout<<"\n\n +5 segundos de tiempo."<<std::endl;
		break;
	case 3: 
		eventQueue.insertSorted({1, gameTime - 7.5f, 7.5f});
		appliedBonus = true;
		std::cout<<"\n\n +7.5 segundos de tiempo."<<std::endl;
		break;
	case 4:
		eventQueue.insertSorted({1, gameTime - 10.0f, 10.0f});
		appliedBonus = true;
		std::cout<<"\n\n +10 segundos de tiempo."<<std::endl;
		break;
	default:
		appliedBonus = false;
		break;
	}
	return appliedBonus;
}

/*
Evento pospuesto
*/
bool GameEngine::punishmentEvent(int rowsReached){
	return false;
}

/*
Evento pospuesto
*/

bool GameEngine::rapidDropEvent(){
	return false;
}

/* 
Permite mover, sostener, rotar y cambiar de pieza (una vez por turno para este último).
*/
void GameEngine::holdCurrentPiece(){
	if(!holdStack.canSwap()) return;
	holdStack.setUsedInTurn(true);
	if(holdStack.isEmpty()){
		holdStack.push(currentPieceType);
		spawnNewPiece();
	} else {
		int aux = holdStack.pop();
		holdStack.push(currentPieceType);
		currentPieceType = aux;
	}
	currentX = 3;
	currentY = 0;
	currentRotation = 0;
	dropTimer = 0.0f;
}

//Controla las teclas mientras se está jugando
void GameEngine::handlePlayInput(sf::Keyboard::Key key){
	switch (key) {
	case sf::Keyboard::Key::Left:
			if (isValidPosition(currentPieceType, currentRotation, currentX - 1, currentY)) {
				currentX--;
				movList.registerMove(MovePiece("LEFT", currentPieceType, currentX, currentY, currentRotation));
			}
		break;
		
	case sf::Keyboard::Key::Right:
			if (isValidPosition(currentPieceType, currentRotation, currentX + 1, currentY)) {
				currentX++;
				movList.registerMove(MovePiece("RIGHT", currentPieceType, currentX, currentY, currentRotation));
			}
		break;
		
		case sf::Keyboard::Key::Up: {
			int nextRotation = (currentRotation + 1) % 4;
			if (isValidPosition(currentPieceType, nextRotation, currentX, currentY)) {
				currentRotation = nextRotation;
				movList.registerMove(MovePiece("ROTATE", currentPieceType, currentX, currentY, currentRotation));
			}
			break;
		}
	
		case sf::Keyboard::Key::Down:
			if (isValidPosition(currentPieceType, currentRotation, currentX, currentY + 1)) {
				currentY++;
				movList.registerMove(MovePiece("DOWN", currentPieceType, currentX, currentY, currentRotation));
			}
		break;
		
		case sf::Keyboard::Key::C:
			holdCurrentPiece();
		break;
		
		case sf::Keyboard::Key::P:
			currentState = GameState::PAUSE;
		break;
	
	default:
		break;
	}
}

//Controla la función de pausa en el juego.
void GameEngine::handlePausedInput(sf::Keyboard::Key key){
	switch(key){
		case sf::Keyboard::Key::P:
			currentState = GameState::PLAYING;
		break;
	default: 
		break;
	}
}
//Controla el historial de los movimientos hechos en la partida una vez que finaliza.
void GameEngine::handleReplayInput(sf::Keyboard::Key key){
	MovePiece outMove;
	if (key == sf::Keyboard::Key::Left || key == sf::Keyboard::Key::Right) {
		isAutoReplay = false; 
	}
	if (key == sf::Keyboard::Key::Left) {
		if (movList.undo(outMove)) {
			currentPieceType = outMove.pieceType;
			currentX = outMove.targetX;
			currentY = outMove.targetY;
			currentRotation = outMove.rotation;
		}
	} else if (key == sf::Keyboard::Key::Right) {
		if (movList.redo(outMove)) {
			currentPieceType = outMove.pieceType;
			currentX = outMove.targetX;
			currentY = outMove.targetY;
			currentRotation = outMove.rotation;
		}
	} else if (key == sf::Keyboard::Key::R) {
		movList.resetToStart();
		isAutoReplay = true;
		replayTimer = 0.0f;
	}
}

GameEngine::GameEngine() {
	window.create(sf::VideoMode({800, 600}), "Tetris");
	currentState = GameState::PLAYING;
	currentPieceType = 0;
	currentX = 0;
	currentY = 0;
	currentRotation = 0;
	score = 0;
	gameTime = 180.0f;
	dropTimer = 0.0f;
	dropInterval = 0.8f;
	replayTimer = 0.0f;
	isGameOver = false;
	isPaused = false;
	isAutoReplay = false;
	scoreManager.loadFile("scores.txt");
	if (font.openFromFile("ariblk.ttf")) {
		fontLoaded = true;
	} else {
		fontLoaded = false;
	}
}

GameEngine::~GameEngine() {
	
}

void GameEngine::run() {
	sf::Clock clock;
	
	while (window.isOpen()) {
		float deltaTime = clock.restart().asSeconds();
		
		processInput();
		update(deltaTime);
		render();
	}
}
