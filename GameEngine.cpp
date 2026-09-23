#include "GameEngine.h"

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

void GameEngine::update(float deltaTime) {
	if(currentState != GameState::PLAYING) return;
	gameTime -= deltaTime;
	
	EventData currentEvent;
	while (eventQueue.showNextEvent(currentEvent) && gameTime <= currentEvent.triggerTime) {
		if (eventQueue.popFront(currentEvent)) {
			std::cout << "Se quito un evento de la cola..." << std::endl;
			if (currentEvent.eventType == 1) {
				gameTime += currentEvent.parameter; 
			}
			
			// Aquí podrías procesar otros tipos de eventos (eventType == 2, etc.) si los hay
		}
	}
	
	if(isTimeExpired()){
		gameTime = 0.0f;
		currentState = GameState::GAME_OVER;
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
			if(linesCleared >= 2){
				timeBonusEvent(linesCleared);
			}
			std::cout << "3. Generando nueva pieza..." << std::endl;
			spawnNewPiece();
			
			std::cout << "4. Pieza generada con exito." << std::endl;
			
			if(!isValidPosition(currentPieceType, currentRotation, currentX, currentY)){
				currentState = GameState::GAME_OVER;
				return;
			}
		}
	}
}

/*
Permite que las piezas creadas sean visibles, así como su animación de eliminación de filas y caída.
*/
void GameEngine::render() {
	window.clear(sf::Color::Black);
	
	if(currentState == GameState::PLAYING || currentState == GameState::PAUSE) {
		const float TILE_SIZE = 30.0f;
		sf::RectangleShape cellShape(sf::Vector2f(TILE_SIZE - 1.0f, TILE_SIZE - 1.0f));
		
		
		for(int r = 0; r < 20; r++) {
			for(int c = 0; c < 10; c++) {
				if(board.isCellOccupied(r, c)) {
					cellShape.setFillColor(sf::Color::Cyan);
					cellShape.setPosition(sf::Vector2f(c * TILE_SIZE, r * TILE_SIZE));
					window.draw(cellShape);
				}
			}
		}
		
		if(!piece.isEmpty()) {
			cellShape.setFillColor(sf::Color::Red);
			for(int r = 0; r < 4; r++) {
				for(int c = 0; c < 4; c++) {
					
					if(PIECE_SHAPES[currentPieceType][currentRotation][r][c] != 0) {
						int targetX = currentX + c;
						int targetY = currentY + r;
						
						if(targetY >= 0 && targetY < 20 && targetX >= 0 && targetX < 10) {
							cellShape.setPosition(sf::Vector2f(targetX * TILE_SIZE, targetY * TILE_SIZE));
							window.draw(cellShape);
						}
					}
				}
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
Permite mover, sostener, rotar y cambiar de pieza (una vez por turno para este último).
*/
void GameEngine::holdCurrentPiece(){
	if(!holdStack.canSwap()) return;
	holdStack.setUsedInTurn(true);
	if(holdStack.isEmpty()){
		holdStack.push(currentPieceType);
		currentPieceType = piece.deletePiece();
	} else {
		int aux = holdStack.pop();
		holdStack.push(currentPieceType);
		currentPieceType = aux;
	}
	currentX = 3;
	currentY = 0;
	currentRotation = 0;
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
//Controla el juego mientras el juego está en pausa.
void GameEngine::handlePausedInput(sf::Keyboard::Key key){
	switch(key){
		case sf::Keyboard::Key::P:
			currentState = GameState::PLAYING;
		break;
	default: 
		break;
	}
}

void GameEngine::handleReplayInput(sf::Keyboard::Key key){
	MovePiece outMove;
	if (key == sf::Keyboard::Key::Left) {
		if (movList.undo(outMove)) {
			currentX = outMove.targetX;
			currentY = outMove.targetY;
			currentRotation = outMove.rotation;
		}
	} else if (key == sf::Keyboard::Key::Right) {
		if (movList.redo(outMove)) {
			currentX = outMove.targetX;
			currentY = outMove.targetY;
			currentRotation = outMove.rotation;
		}
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
	isGameOver = false;
	isPaused = false;
}

GameEngine::~GameEngine() {
	
}

void GameEngine::run() {
	sf::Clock clock;
	spawnNewPiece();
	
	while (window.isOpen()) {
		float deltaTime = clock.restart().asSeconds();
		
		processInput();
		update(deltaTime);
		render();
	}
}
