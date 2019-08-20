#include <iostream>
#include <cstdlib>
#include <chrono>
#include <random>
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace std;

const short SCALE = 40;
const int CANVAS_SIZE_Y = 10.8 * SCALE;
const int CANVAS_SIZE_X = 19.2 * SCALE;
const short FPS = 60;
const short DROP_SPEED = 7;
const float POWDER_DENSITY = 0.75;
bool drop_powder = false;
bool increase_size = false;
bool decrease_size = false;
int cursor_pos[2];
int circle_size = 5;
const sf::Color BLACK = sf::Color::Black;
const sf::Color WHITE = sf::Color::White;
sf::VertexArray vertices(sf::Points, CANVAS_SIZE_X * CANVAS_SIZE_Y);
class Powder;
list<Powder> powder_list;
sf::RenderWindow window(sf::VideoMode(CANVAS_SIZE_X, CANVAS_SIZE_Y, 24U), "simple powder toy", sf::Style::Close | sf::Style::Titlebar);

class Powder
{
public:
	int y;
	int x;
	int move_x;
	int move_y;
	const sf::Color color;
	default_random_engine engine;
	normal_distribution<double> distribution{ 0.0, 1.75 };

	Powder(int cursor_pos[], int y, int x)
		: color(WHITE),
		y(cursor_pos[0] + y),
		x(cursor_pos[1] + x),
		move_x(0), move_y(0)
	{
		if (x > CANVAS_SIZE_X - 1)
			x = CANVAS_SIZE_X - 1;
		else if (x < 0)
			x = 0;

		if (y > CANVAS_SIZE_Y - 1)
			y = CANVAS_SIZE_Y - 1;
		else if (y < 0)
			y = 0;
	}

	bool can_drop()
	{
		if (x + move_x >= CANVAS_SIZE_X || y + move_y >= CANVAS_SIZE_Y || x + move_x < 0 || y + move_y < 0)
			return false;
		return vertices[CANVAS_SIZE_X * (y + move_y) + x + move_x].color == BLACK;
	}

	void find_drop_values()
	{
		while (!can_drop())
		{
			if (move_x < 0)
				move_x++;
			else if (move_x > 0)
				move_x--;
			if (!can_drop() && move_y > 0)
				move_y--;
			
			if (!move_x && !move_y)
				break;
		}
	}

	void fall()
	{
		move_x = round(distribution(engine));
		move_y = DROP_SPEED;
		find_drop_values();
		vertices[y * CANVAS_SIZE_X + x].color = BLACK;
		x += move_x;
		y += move_y;
		vertices[CANVAS_SIZE_X * y + x].color = WHITE;
	}
};

void handle_events(sf::RenderWindow& window)
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseMoved:
			cursor_pos[0] = event.mouseMove.y;
			cursor_pos[1] = event.mouseMove.x;

			if (cursor_pos[0] >= CANVAS_SIZE_Y)
				cursor_pos[0] = CANVAS_SIZE_Y - 1;
			else if (cursor_pos[0] < 0)
				cursor_pos[0] = 0;

			if (cursor_pos[1] >= CANVAS_SIZE_X)
				cursor_pos[1] = CANVAS_SIZE_X - 1;
			else if (cursor_pos[1] < 0)
				cursor_pos[1] = 0;

			break;

		case sf::Event::Closed:
			window.close();
			break;

		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
				drop_powder = true;
			break;

		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
				drop_powder = false;
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::W)
				increase_size = true;
			else if (event.key.code == sf::Keyboard::S)
				decrease_size = true;
			break;

		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::W)
				increase_size = false;
			else if (event.key.code == sf::Keyboard::S)
				decrease_size = false;
			break;
		}
	}
}

int main()
{
	cout << "Starting...";
	window.setFramerateLimit(FPS);
	for (int y = 0; y < CANVAS_SIZE_Y; y++)
	{
		for (int x = 0; x < CANVAS_SIZE_X; x++)
		{
			vertices[CANVAS_SIZE_X * y + x] = sf::Vertex(sf::Vector2f(x, y), sf::Color::Black);
		}
	}

	srand(time(0));
	while (window.isOpen())
	{
		handle_events(window);
		if (increase_size)
			circle_size++;
		if (decrease_size)
			circle_size--;

		if (drop_powder)
		{
			for (int i = 0; i < circle_size; i+=2)
			{
				for (int j = 0; j < circle_size; j+=2)
				{
					powder_list.push_front(Powder(cursor_pos, i, j));
				}
			}
		}
		for (Powder& powder : powder_list)
		{
			powder.engine.seed(chrono::system_clock::now().time_since_epoch().count());
			powder.fall();
		}
		window.draw(vertices);
		window.display();
	}
}
