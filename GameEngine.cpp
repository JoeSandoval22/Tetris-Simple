#include "GameEngine.h"

void GameEngine::processInput() {
	while (const std::optional<sf::Event> event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
		if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
			if (currentState == GameState::PLAYING) {
				handlePlayInput(keyPress->code);
			} else if (currentState == GameState::GAME_OVER) {
				handleReplayInput(keyPress->code);
			}
		}
	}
}

void GameEngine::update(float deltaTime) {
	if(currentState != GameState::PLAYING) return;
	gameTime -= deltaTime;
	
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
	gameTime = 10.0f;
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
