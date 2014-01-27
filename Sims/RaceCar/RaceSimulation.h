#pragma once
#include "../../sEvolution/sSimulation.h"
#include "../../sUtils/perlin.h"



class sRaceFitness : public sFitnessFunction
{
public: 
	sRaceFitness() : useSpeed(true){}
	bool useSpeed;
	 float operator()(sPhenotype *phenotype)
	{
		float distance = phenotype->getPosition().x;
		float energy = ((Worm*)phenotype)->getEneryUsed() + 2;
		float speed = abs(distance) / float(phenotype->lifeTime + 1);

		if(useSpeed){
			//return distance * speed;
			return 100.f * speed * distance;
		} else {
			return distance;
		}
	}
};

template <typename PhenotypeClass>
class RaceSimulation : public sSimulation
{

public:

	RaceSimulation()
	{
		(void)static_cast<sPhenotype*>((PhenotypeClass*)0);

		worldWidth = 600;
		worldOffset = 6;
		worldOffset_y = 5;
		populationSize = 50;
		randomizeEnvironment = true;
		minRoughness = 4;
		maxRoughness = 4;
		groundSegmentSize = 0.5f;
		groundType = GroundType::PerlinNoise;
		perlinOctaves = 3;
		perlinFrequency = 8;
		groundFrequency = 2.f;

		ground.vertexSplitBaseWeight = 1.f;
		ground.doubleSolveWeight = 2.f;
		ground.segmentSplitBaseWeight = 10.f;
		ground.lengthWeight = 0.5f;
		ground.dotWeight = 1.f;

		world.setGroundBody(&ground);

		fitnessFunction.useSpeed = false;

	}

	~RaceSimulation()
	{
		for(int i = 0; i < populationSize; i++){
			delete population[i];
		}
	}



	// Simulation Settings
	float worldWidth;
	float worldOffset;
	float worldOffset_y;
	int populationSize;
	bool randomizeEnvironment;

	static enum GroundType	{ PerlinNoise, SinWaves };
	GroundType groundType;
	float minRoughness;
	float maxRoughness;
	float perlinFrequency;
	float perlinOctaves;
	float groundFrequency;
	float groundSegmentSize;

	sRaceFitness fitnessFunction;

protected:

	void initPhenotypes()
	{
		for(int i = 0; i < populationSize; i++){
			sPhenotype *phenotype = static_cast<sPhenotype*>(new PhenotypeClass);
			phenotype->fitnessFunction = &fitnessFunction;
			phenotype->init(world);
			population.addPhenotype(phenotype);
			//world.addContactListener(car, &car->chassis);
			world.add(phenotype);
			if(!i){
				phenotype->neuralNet.printStats();
				phenotype->genome.printStats();
			}
		}
		population.printStats();

	}


	// Return whether the simulation is finished
	// In this case it's when all phenotypes are no longer active/alive
	bool isFinished()
	{
		for(int i = 0; i < population.size(); i++){
			if(population[i]->getPosition().x > worldWidth - worldOffset){
				population[i]->die();
				return true;
			}
		}
		
		// Speed up simulation if leader is not alive
		//if(leader != nullptr)speedUp = !leader->alive;



		for(int i = 0; i < population.size(); i++){
			if(population[i]->alive) return false;
		}
		return true;
	}

	// Build / Rebuild environment
	// Builds the ground based on roughness inputs
	void buildEnvironment()
	{
		if(ground.isInWorld()){
			// Don't regenrate if we don;t randomize
			if(!randomizeEnvironment)return;

			// Remove and reset
			world.remove(&ground);
			ground.resetShape();
		}

		Perlin perlin(perlinOctaves, perlinFrequency, 0.5, sRandom::getInt(0,100000));


		randOffset1 = sRandom::getFloat(0,worldWidth);
		randOffset2 = sRandom::getFloat(0,worldWidth);
		randOffset3 = sRandom::getFloat(0,worldWidth);



		float32 h = sRandom::getFloat(minRoughness, maxRoughness);
		float32 start_h = getHeightValue(worldOffset, perlin) * h;
		ground.add(worldWidth, h + 1.f);
		ground.add(-2.f, h + 1.f);
		ground.add(-2.f, start_h - 3);
		ground.add(-1.f, start_h - 3);
		ground.add(-1.f, start_h);
		ground.add(worldOffset, start_h);
		ground.setPosition(3.f - worldOffset, worldOffset_y - start_h);
		int index = 0;
		float rampup = 50;
		for(float32 x = worldOffset; x <= worldWidth; x += groundSegmentSize){

			float y = getHeightValue(x, perlin) * h;
			if(x < rampup){
				y = start_h + (y - start_h) * (x / rampup);
			}
			ground.add(x, y);
		}
		ground.finalizeShape();
		ground.setType(STATIC_BODY);
		ground.setFriction(1);
		ground.setRestitution(0);
		world.add(&ground);

		speedUp = false;
	}


private:

	// returns a value between 0 - 1;
	float getHeightValue(float x, Perlin &perlin)
	{
		x *= groundFrequency;
		float v;
		if(groundType == GroundType::PerlinNoise){
			v = perlin.Get(x / worldWidth, 0);
		} else {
			float w1 = sin(randOffset1 + x / worldWidth * 59.f) * 0.25f;
			float w2 = sin(randOffset2 + x / worldWidth * 231.f) * 0.15f;
			float w3 = sin(randOffset3 + x / worldWidth * 851.f) * 0.05f;
			v = w1 + w2 + w3;
		}
		return 0.5f + v;
	}
	float randOffset1, randOffset2, randOffset3;

	sConcavePolygon ground;
};