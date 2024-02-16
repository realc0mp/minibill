
#include <cassert>
#include <cmath>
#include <array>

#include "../framework/scene.hpp"
#include "../framework/game.hpp"
#include "../framework/engine.hpp"


//-------------------------------------------------------
//	Basic Vector2 class
//-------------------------------------------------------

class Vector2
{
public:
	float x = 0.f;
	float y = 0.f;

	constexpr Vector2() = default;
	constexpr Vector2( float vx, float vy );
	constexpr Vector2( Vector2 const &other ) = default;
};


constexpr Vector2::Vector2( float vx, float vy ) :
	x( vx ),
	y( vy )
{
}


//-------------------------------------------------------
//	game parameters
//-------------------------------------------------------

namespace Params
{
	namespace System
	{
		constexpr int targetFPS = 60;
	}

	namespace Table
	{
		constexpr float width = 15.f;
		constexpr float height = 8.f;
		constexpr float pocketRadius = 0.4f;

		static constexpr std::array< Vector2, 6 > pocketsPositions =
		{
			Vector2{ -0.5f * width, -0.5f * height },
			Vector2{ 0.f, -0.5f * height },
			Vector2{ 0.5f * width, -0.5f * height },
			Vector2{ -0.5f * width, 0.5f * height },
			Vector2{ 0.f, 0.5f * height },
			Vector2{ 0.5f * width, 0.5f * height }
		};

		static constexpr std::array< Vector2, 7 > ballsPositions =
		{
			// player ball
			Vector2( -0.3f * width, 0.f ),
			// other balls
			Vector2( 0.2f * width, 0.f ),
			Vector2( 0.25f * width, 0.05f * height ),
			Vector2( 0.25f * width, -0.05f * height ),
			Vector2( 0.3f * width, 0.1f * height ),
			Vector2( 0.3f * width, 0.f ),
			Vector2( 0.3f * width, -0.1f * height )
		};
	}

	namespace Ball
	{
		constexpr float radius = 0.3f;
	}

	namespace Shot
	{
		constexpr float chargeTime = 1.f;
	}
}

//-------------------------------------------------------
//	Table logic
//-------------------------------------------------------

class Table
{
public:
	Table() = default;
	Table( Table const& ) = delete;
	void init();
	void deinit();
	void moveBalls() {}
	void isInsideHole();
	void setBallPosition(int number, float x, float y);
private:
	std::array< Scene::Mesh*, 6 > pockets = {};
	std::array< Scene::Mesh*, 7 > balls = {};
};


void Table::init()
{
	for ( int i = 0; i < 6; i++ )
	{
		assert( !pockets[ i ] );
		pockets[ i ] = Scene::createPocketMesh( Params::Table::pocketRadius );
		Scene::placeMesh( pockets[ i ], Params::Table::pocketsPositions[ i ].x, Params::Table::pocketsPositions[ i ].y, 0.f );
	}

	for ( int i = 0; i < 7; i++ )
	{
		assert( !balls[ i ] );
		balls[ i ] = Scene::createBallMesh( Params::Ball::radius );
		Scene::placeMesh( balls[ i ], Params::Table::ballsPositions[ i ].x, Params::Table::ballsPositions[ i ].y, 0.f );
	}
}


void Table::deinit()
{
	for ( Scene::Mesh* mesh : pockets )
		Scene::destroyMesh( mesh );
	for ( Scene::Mesh* mesh : balls )
		Scene::destroyMesh( mesh );
	pockets = {};
	balls = {};
}

void Table::setBallPosition(int number, float x, float y) {
	Scene::placeMesh(balls[number], x, y, 0);
}

void Table::isInsideHole() {
	/*for (auto ball : balls) {
		for (auto pocket : pockets) {}
	}*/
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 7; j++) {
			Scene::Mesh& boofPocket = *pockets[i];
			Scene::Mesh& boofBall = *balls[j];
			float dx = boofBall->position.x;
		}
	}
}

//-------------------------------------------------------
//	game public interface
//-------------------------------------------------------

namespace Game
{
	Table table;

	bool isChargingShot = false;
	float shotChargeProgress = 0.f;


	void init()
	{
		Engine::setTargetFPS( Params::System::targetFPS );
		Scene::setupBackground( Params::Table::width, Params::Table::height );
		table.init();
	}


	void deinit()
	{
		table.deinit();
	}


	void update( float dt )
	{
		if ( isChargingShot )
			shotChargeProgress = std::min( shotChargeProgress + dt / Params::Shot::chargeTime, 1.f );
		Scene::updateProgressBar( shotChargeProgress );
	}


	void mouseButtonPressed( float x, float y )
	{
		isChargingShot = true;
	}

	void ballMove(float x, float y) {
		table.setBallPosition(0, x, y);
	}

	void mouseButtonReleased( float x, float y )
	{
		// TODO: implement billiard logic here

		isChargingShot = false;
		shotChargeProgress = 0.f;
		ballMove(x, y);
	}

}
