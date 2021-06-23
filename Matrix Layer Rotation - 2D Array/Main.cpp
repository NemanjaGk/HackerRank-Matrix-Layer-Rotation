#include <iostream>
#include <iomanip>
#include <algorithm>

#pragma region Position signature

struct Position {
	short row;
	short column;
	friend bool operator==(const Position& position1, const Position& position2);
	Position& operator=(const Position& assign);
};

#pragma endregion

#pragma region Position implementation

Position& Position::operator=(const Position& assign) {
	this->row = assign.row;
	this->column = assign.column;
	return *this;
}

bool operator==(const Position& position1, const Position& position2) {
	return position1.row == position2.row && position1.column == position2.column;
}

#pragma endregion

#pragma region MatrixLayerRotation signature

class MatrixLayerRotation {
public:
	MatrixLayerRotation();
	~MatrixLayerRotation();

	MatrixLayerRotation* rotateLayers();
	void printMatrix() const;

private:
	void readProperties();
	void readData();
	void rotateSingleLayer(const short& currentLayer);
	Position getNextPosition(const Position& currentPosition, const short& currentLayer, const short& rotations) const;
	Position displace(Position& startPosition, const short& currentLayer) const;

private:
	int** m_Matrix;
	short m_NumberOfRows,
		m_NumberOfColumns;
	int m_Rotations;
};

#pragma endregion

#pragma region MatrixLayerRotation implementation

#pragma region Construct

MatrixLayerRotation::MatrixLayerRotation() {
	this->readProperties();
	this->readData();
}

MatrixLayerRotation::~MatrixLayerRotation() {
	for (int i = 0; i < this->m_NumberOfRows; ++i)
		delete this->m_Matrix[i];
	delete this->m_Matrix;
	this->m_Matrix = nullptr;
}

#pragma endregion 

#pragma region Read

void MatrixLayerRotation::readProperties() {
	std::cin >> this->m_NumberOfRows >> this->m_NumberOfColumns >> this->m_Rotations;
}

void MatrixLayerRotation::readData() {
	this->m_Matrix = new int* [this->m_NumberOfRows];
	for (int i = 0; i < this->m_NumberOfRows; ++i) {
		this->m_Matrix[i] = new int[this->m_NumberOfColumns];
		for (int j = 0; j < this->m_NumberOfColumns; ++j)
			std::cin >> this->m_Matrix[i][j];
	}
}

#pragma endregion

#pragma region Write

void MatrixLayerRotation::printMatrix() const {
	for (int i = 0; i < this->m_NumberOfRows; ++i) {
		for (int j = 0; j < this->m_NumberOfColumns; ++j)
			std::cout << this->m_Matrix[i][j] << ' ';
		std::cout << std::endl;
	}
}

#pragma endregion

#pragma region Rotate

MatrixLayerRotation* MatrixLayerRotation::rotateLayers() {
	short numberOfLayers = (std::min(this->m_NumberOfRows, this->m_NumberOfColumns) + 1) / 2;
	for (int currentLayer = 0; currentLayer < numberOfLayers; ++currentLayer)
		this->rotateSingleLayer(currentLayer);
	return this;
}

void MatrixLayerRotation::rotateSingleLayer(const short& currentLayer) {
	short numberOfLayerMembers = 2 * (this->m_NumberOfRows + this->m_NumberOfColumns - 4 * currentLayer - 2);
	const short rotations = this->m_Rotations % numberOfLayerMembers;
	Position startPosition{ currentLayer, currentLayer },
		currentPosition{ currentLayer, currentLayer },
		nextPosition;
	while (numberOfLayerMembers--) {
		nextPosition = this->getNextPosition(currentPosition, currentLayer, rotations);
		if (nextPosition == startPosition)
			nextPosition = this->displace(startPosition, currentLayer);
		else
			std::swap(this->m_Matrix[currentPosition.row][currentPosition.column], this->m_Matrix[nextPosition.row][nextPosition.column]);
		currentPosition = nextPosition;
	}
}

#pragma endregion

#pragma region Find position

Position MatrixLayerRotation::getNextPosition(const Position& currentPosition, const short& currentLayer, const short& rotations) const {
	Position nextPosition = currentPosition;
	short componentTopRight,
		componentRightBottom;
	const short& componentBottomLeft = currentLayer,
		& componentLeftTop = currentLayer;
	short steps = rotations,
		change;

	while (steps) {
		componentTopRight = this->m_NumberOfColumns - currentLayer - 1;
		componentRightBottom = this->m_NumberOfRows - currentLayer - 1;
		// Top
		if (nextPosition.row == componentLeftTop && nextPosition.column < componentTopRight) {
			change = (nextPosition.column + steps <= componentTopRight) ? steps : componentTopRight - nextPosition.column;
			nextPosition.column += change;
		}
		// Right
		else if (nextPosition.column == componentTopRight && nextPosition.row < componentRightBottom) {
			change = (nextPosition.row + steps <= componentRightBottom) ? steps : componentRightBottom - nextPosition.row;
			nextPosition.row += change;
		}
		// Bottom
		else if (nextPosition.row == componentRightBottom && nextPosition.column > componentBottomLeft) {
			change = (nextPosition.column - steps >= componentBottomLeft) ? steps : nextPosition.column - componentBottomLeft;
			nextPosition.column -= change;
		}
		// Left
		else if (nextPosition.column == componentBottomLeft && nextPosition.row > componentLeftTop) {
			change = (nextPosition.row - steps >= componentLeftTop) ? steps : nextPosition.row - componentLeftTop;
			nextPosition.row -= change;
		}
		steps -= change;
	}
	return nextPosition;
}

Position MatrixLayerRotation::displace(Position& startPosition, const short& currentLayer) const {
	return startPosition = this->getNextPosition(startPosition, currentLayer, 1);
}

#pragma endregion

#pragma endregion

#pragma region Main function

int main() {
	MatrixLayerRotation* matrixLayerRotation = new MatrixLayerRotation();
	matrixLayerRotation->rotateLayers()->printMatrix();
	delete matrixLayerRotation;
}

#pragma endregion
