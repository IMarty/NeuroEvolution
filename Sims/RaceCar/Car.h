#pragma once
#include "..\..\sPhysics\sPhysics.h"
#include "..\..\sPhysics\sWheelJoint.h"
#include "..\..\sEvolution\sPhenotype.h"
#include "..\..\sNeuralNet\sNeuralNet.h"
using std::vector;

class Car : public sPhenotype, public sContactListener
{

public:

	Car()
	{

	}


	void init(sWorld &world)
	{

		//---------------------------------------------------------------------------------
		// Phenotype Initialization
		//---------------------------------------------------------------------------------

		// Add bodyparts to container
		add(&frontWheel);
		add(&rearWheel);
		add(&chassis);
		add(&frontSuspension);
		add(&rearSuspension);


		// Filters out all collisions with parts and other cars
		b2Filter filter;
		filter.categoryBits = 0x02;
		filter.maskBits = 0x01;

		// Chassis
		chassis.setFilter(filter);
		chassis.setLinearDamping(0.1f);
		chassis.setAngularDamping(0.1f);

		// Front wheel
		frontWheel.setFilter(filter);
		frontWheel.setFriction(1.f);
		frontWheel.setRestitution(0.0f);
		frontWheel.setAngularDamping(0.7f);
		frontWheel.setLinearDamping(0.1f);

		// Rear wheel
		rearWheel.setFilter(filter);
		rearWheel.setFriction(1.f);
		rearWheel.setRestitution(0.0f);
		rearWheel.setAngularDamping(0.7f);
		rearWheel.setLinearDamping(0.1f);

		// Front suspension
		frontSuspension.setBodies(&chassis, &frontWheel);
		frontSuspension.setEnableMotor(true);

		// Rear suspension
		rearSuspension.setBodies(&chassis, &rearWheel);
		rearSuspension.setEnableMotor(true);

		// Add listeners for contact events
		world.addContactListener(this, &chassis);
		world.addContactListener(this, &frontWheel);
		world.addContactListener(this, &rearWheel);		
		
		//---------------------------------------------------------------------------------
		// Genome Definition
		//---------------------------------------------------------------------------------

		// Chassis
		/* High variation
		genome.addGene("scale", 0.7, 1.3);
		genome.addGene("chassisDensity", 1, 1);
		genome.addGene("chassisScale_x", 0.2, 2);
		genome.addGene("chassisScale_y", 0.5, 2.0);


		// Front Wheel
		genome.addGene("frontWheelRadius", 0.2, 1.5);
		genome.addGene("frontWheelDensity", 1, 1);
		genome.addGene("frontWheelPosition_x", -2.5, 2.5, 30);
		genome.addGene("frontWheelPosition_y", -2, 2, 30);
		genome.addGene("frontWheelAnchor_x", -1.8, 1.8, 30);
		genome.addGene("frontWheelAnchor_y", -1 ,0);

		// Front Motor
		genome.addGene("frontMotorTorque", 0, 300);
		genome.addGene("frontMotorSpeed", 0, 100);
		
		// Front Suspension
		genome.addGene("frontFrequencyHz", 0.2, 6);
		genome.addGene("frontDampingRatio", 0.2, 0.999);

		genome.copyGene("rearWheelRadius", "frontWheelRadius");
		genome.copyGene("rearWheelDensity", "frontWheelDensity");
		genome.copyGene("rearWheelPosition_x", "frontWheelPosition_x", GENE_INVERSE);
		genome.copyGene("rearWheelPosition_y", "frontWheelPosition_y");
		genome.copyGene("rearWheelAnchor_x", "frontWheelAnchor_x", GENE_INVERSE);
		genome.copyGene("rearWheelAnchor_y", "frontWheelAnchor_y");

		// Rear Motor
		genome.copyGene("rearMotorTorque", "frontMotorTorque");
		genome.copyGene("rearMotorSpeed", "frontMotorSpeed");

		// Rear Suspension
		genome.copyGene("rearFrequencyHz", "frontFrequencyHz");
		genome.copyGene("rearDampingRatio", "frontDampingRatio");
		*/
		/*
		// No variation
		genome.addGene("scale", 1, 1);
		genome.addGene("chassisDensity", 2, 2);
		genome.addGene("chassisScale_x", 1.1, 1.1);
		genome.addGene("chassisScale_y", 1.1, 1.1);


		// Front Wheel
		genome.addGene("frontWheelRadius", 1, 1);
		genome.addGene("frontWheelDensity", 1, 1);
		genome.addGene("frontWheelPosition_x", 2,2);
		genome.addGene("frontWheelPosition_y", 1,1);
		genome.addGene("frontWheelAnchor_x", 1.5, 1.5);
		genome.addGene("frontWheelAnchor_y", -1 ,-1);

		// Front Motor
		genome.addGene("frontMotorTorque", 40, 40);
		genome.addGene("frontMotorSpeed", 40, 40);
		
		// Front Suspension
		genome.addGene("frontFrequencyHz", 3, 3);
		genome.addGene("frontDampingRatio", 0.7, 0.7);
		

		// Rear Wheel
		//genome.copyGene("rearWheelRadius", "frontWheelRadius");
		genome.addGene("rearWheelRadius", 1.4,1.4);
		genome.copyGene("rearWheelDensity", "frontWheelDensity");
		genome.copyGene("rearWheelPosition_x", "frontWheelPosition_x", GENE_INVERSE);
		genome.copyGene("rearWheelPosition_y", "frontWheelPosition_y");
		genome.copyGene("rearWheelAnchor_x", "frontWheelAnchor_x", GENE_INVERSE);
		genome.copyGene("rearWheelAnchor_y", "frontWheelAnchor_y");

		// Rear Motor
		genome.addGene("rearMotorTorque", 200, 200);
		genome.addGene("rearMotorSpeed", 200, 200);

		// Rear Suspension
		genome.copyGene("rearFrequencyHz", "frontFrequencyHz");
		genome.copyGene("rearDampingRatio", "frontDampingRatio");
		*/

		// Small variation, generally valid phenotype
		genome.addGene("scale", 1, 1);
		genome.addGene("chassisDensity", 1.7, 1.7);
		genome.addGene("chassisScale_x", 0.9, 1.3);
		genome.addGene("chassisScale_y", 0.9, 1.3);


		// Front Wheel
		genome.addGene("frontWheelRadius", 0.5, 1.3);
		genome.addGene("frontWheelDensity", 1, 1);
		genome.addGene("frontWheelPosition_x", 1.3,2.2);
		genome.addGene("frontWheelPosition_y", 0.0, 1.5);
		genome.addGene("frontWheelAnchor_x", 1.2, 1.8);
		genome.addGene("frontWheelAnchor_y", -1.3 ,-0.0);

		// Front Motor
		genome.addGene("frontMotorTorque", 100, 100);
		genome.addGene("frontMotorSpeed", 100, 100);
		
		// Front Suspension
		genome.addGene("frontFrequencyHz", 1.1, 5);
		genome.addGene("frontDampingRatio", 0.5, 0.95);
		

		// Rear Wheel
		//genome.copyGene("rearWheelRadius", "frontWheelRadius");
		genome.addGene("rearWheelRadius", 0.6, 1.6);
		genome.copyGene("rearWheelDensity", "frontWheelDensity");
		genome.copyGene("rearWheelPosition_x", "frontWheelPosition_x", GENE_INVERSE);
		genome.copyGene("rearWheelPosition_y", "frontWheelPosition_y");
		genome.copyGene("rearWheelAnchor_x", "frontWheelAnchor_x", GENE_INVERSE);
		genome.copyGene("rearWheelAnchor_y", "frontWheelAnchor_y");

		// Rear Motor
		genome.addGene("rearMotorTorque", 200, 200);
		genome.addGene("rearMotorSpeed", 200, 200);

		// Rear Suspension
		genome.copyGene("rearFrequencyHz", "frontFrequencyHz");
		genome.copyGene("rearDampingRatio", "frontDampingRatio");


		//---------------------------------------------------------------------------------
		// Neural Network Definition
		//---------------------------------------------------------------------------------
 
		neuralNet.setInputCount(7);
		neuralNet.setOutputCount(2);
		neuralNet.setHiddenLayerCount(1);
		neuralNet.setHiddenLayerSize(0,8);
		//neuralNet.setHiddenLayerSize(1,4);
		neuralNet.setMaxBias(1);
		neuralNet.setMaxWeight(1);
		neuralNet.create(genome);


	}


	//=====================================================================================
	// sPhenotype implementation
	//=====================================================================================

	// Rebuilds the phenotype from it's genome
	void build(sWorld &world)
	{

		//---------------------------------------------------------------------------------
		// Phenotype Generation from Genome
		//---------------------------------------------------------------------------------

		// Make bodies dynamic again
		frontWheel.setType(DYNAMIC_BODY);
		rearWheel.setType(DYNAMIC_BODY);
		chassis.setType(DYNAMIC_BODY);

		// Get scale values
		float scale = genome.getValue("scale");
		float32 sx = genome.getValue("chassisScale_x") * scale;
		float32 sy = genome.getValue("chassisScale_y") * scale;

		// Set up chassis body
		chassis.zeroState();
		chassis.resetShape();
		chassis.setDensity(genome.getValue("chassisDensity"));
		
		chassis.add(1.7f * sx, 0.f * sy);
		chassis.add(1.8f * sx, -0.5f * sy);
		chassis.add(1.5f * sx, -0.6f * sy);
		chassis.add(1.f * sx, -1.f * sy);
		chassis.add(-1.f * sx, -1.f * sy);
		chassis.add(-1.5f * sx, -0.6f * sy);
		chassis.add(-1.8f * sx, -0.5f * sy);
		chassis.add(-1.7f * sx, 0.f * sy);
		chassis.finalizeShape();

		// Front wheel
		frontWheel.zeroState();
		frontWheel.setRadius(genome.getValue("frontWheelRadius") * scale);
		frontWheel.setPosition(b2Vec2(genome.getValue("frontWheelPosition_x") * scale, 
			                          genome.getValue("frontWheelPosition_y") * scale));
		frontWheel.setDensity(genome.getValue("frontWheelDensity"));

		// Front suspension
		frontAnchor.x = genome.getValue("frontWheelAnchor_x") * sx;
		frontAnchor.y = genome.getValue("frontWheelAnchor_y") * sy;
		frontSuspension.setAnchorA(frontWheel.getPosition());
		frontSuspension.setAnchorB(frontAnchor);
		frontSuspension.setFrequencyHz(genome.getValue("frontFrequencyHz"));
		frontSuspension.setDampingRatio(genome.getValue("frontDampingRatio"));
		frontSuspension.setAxis(frontAnchor - frontWheel.getPosition());

		// Rear Wheel
		rearWheel.zeroState();
		rearWheel.setRadius(genome.getValue("rearWheelRadius") * scale);
		rearWheel.setPosition(b2Vec2(genome.getValue("rearWheelPosition_x") * scale, 
			                         genome.getValue("rearWheelPosition_y") * scale));
		rearWheel.setDensity(genome.getValue("rearWheelDensity"));

		// Rear suspension
		rearAnchor.x = genome.getValue("rearWheelAnchor_x") * sx;
		rearAnchor.y = genome.getValue("rearWheelAnchor_y") * sy;
		rearSuspension.setAnchorA(rearWheel.getPosition());
		rearSuspension.setAnchorB(rearAnchor);
		rearSuspension.setFrequencyHz(genome.getValue("rearFrequencyHz"));
		rearSuspension.setDampingRatio(genome.getValue("rearDampingRatio"));
		rearSuspension.setAxis(rearAnchor - rearWheel.getPosition());


		// Calculate values used in fitness function
		payloadMass = genome.getValue("chassisDensity") * sx * sy;
		wheelMass = genome.getValue("rearWheelDensity") * genome.getValue("rearWheelRadius") * scale * genome.getValue("rearWheelRadius") * scale;
		wheelMass += genome.getValue("frontWheelDensity") * genome.getValue("frontWheelRadius") * scale * genome.getValue("frontWheelRadius") * scale;

		// Reset neural net inputs
		neuralNet.prepare();
		neuralNet.setInput(0,0);
		neuralNet.setInput(1,0);
		neuralNet.setInput(2,0);
		neuralNet.setInput(3,0);

		// Initialise some values for simulation
		setThrottle(0,0);
		fitnessModifier = 1;
		progressPosition = chassis.getPosition().x;
		progressDelay = 0;
		deferDeath = false;
		frontWheelContact = false;
		rearWheelContact = false;
		lastLinearVelocity.Set(0,0);
		lastAngularVelocity = 0;

		if(isElite()){
			chassis.setCustomColor(b2Color(0,1,1));
		} else {
			chassis.setCustomColor(b2Color(0,0,0));
		}
	}

	
	// Returns the position of this individual
	virtual b2Vec2 getPosition()
	{
		return chassis.getPosition();
	}
	virtual b2Vec2 getVelocity()
	{
		return chassis.getLinearVelocity();
	}

	//---------------------------------------------------------------------------------
	// Phenotype Fitness Function
	//---------------------------------------------------------------------------------
	
	virtual float getFitness() 
	{
		float distance = chassis.getPosition().x;
		//if(distance < 0)fitnessModifier = 0;
		float speed = abs(distance) / float(lifeTime + 1);
		//return float(pow(2, fitnessModifier * distance * speed * 0.1f)); //payloadMass;// / wheelMass;
		return fitnessModifier * distance * speed; //payloadMass;// / wheelMass;
	}


	// Called before world physics is advanced
	void step()
	{
		assert(alive);

		if(deferDeath){
			die();
			deferDeath = false;
			return;
		}
		

		//---------------------------------------------------------------------------------
		// Neural Net behaviour
		//---------------------------------------------------------------------------------

		int index = 0;

		// Set chassis angle input
		float angle = chassis.getAngle() / b2_pi;
		while(angle > 1)angle -= 2.f;
		while(angle < -1)angle += 2.f;
		neuralNet.setInput(index++, angle);

		// Set angular velocity input
		neuralNet.setInput(index++, chassis.getAngularVelocity() * 0.6f);

		// Angular acceleration input
		float aa = getAngularAcceleration();
		//if(isLeader())printf("%f \n", aa);
		neuralNet.setInput(index++, aa * 10.f);


		// Front Wheel sensor input
		float v = neuralNet.getInput(2);
		float g = frontWheelContact ? 1.f : -1.f;
		neuralNet.setInput(index++, v + (g - v) * 0.2f);

		// rear wheel sensor input
		v = neuralNet.getInput(3);
		g = rearWheelContact ? 1.f : -1.f;
		neuralNet.setInput(index++, v + (g - v) * 0.2f);

		// Linear acceleration input
		b2Vec2 acceration = getLinearAcceleration();
		neuralNet.setInput(index++, acceration.x);
		neuralNet.setInput(index++, acceration.y);


	
		// Run neural net ////////////////////////////////
		neuralNet.run();
		//////////////////////////////////////////////////


		// Get outputs, these control the throttle
		float front_acc = neuralNet.getOutput(0);
		float rear_acc = neuralNet.getOutput(1);
		
		// Color the wheel according to throttle values
		frontWheel.setCustomColor(getAccelatorColor(front_acc));
		rearWheel.setCustomColor(getAccelatorColor(rear_acc));

		// Apply throttle, ramp up slowly during the first 2 seconds
		if(lifeTime <= 120){
			float f = lifeTime / 240.f;
			setThrottle(f * front_acc, f * rear_acc);
		} else {
			setThrottle(front_acc, rear_acc);
		}

		if(isLeader()){

		}

		//---------------------------------------------------------------------------------
		// Death conditions
		//---------------------------------------------------------------------------------

		// check for joint instability
		b2Vec2 axis1 = chassis.m_body->GetLocalPoint(rearWheel.getPosition());
		axis1 -= rearSuspension.getAnchorA();
		b2Vec2 axis2 = chassis.m_body->GetLocalPoint(frontWheel.getPosition());
		axis2 -= frontSuspension.getAnchorA();
		if(axis1.Length() > 4.f || axis2.Length() > 4.f){
			printf("infant death\n");
			fitnessModifier = 0.0;
			//genome.print();
			die();
		}

		// Kill off if it's upside down
		float a = chassis.getAngle();
		while(a > b2_pi)a -= b2_pi * 2;
		while(a < -b2_pi)a += b2_pi;
		if(abs(a) > b2_pi * 0.85){
			die();
			fitnessModifier = 0.5f;
			return;
		}
		
		// Kill it off if it doesn't make progress for some time
		if(chassis.getPosition().x > progressPosition){
			progressDelay = 0;
			progressPosition = chassis.getPosition().x + 0.3f;
		} else {
			if(++progressDelay > progressTimeout){
				die();
				return;
			}
		}

	}


	// Sets bodies to static
	void destroy(sWorld &world) // sPhenotype implementation
	{
		setCustomColor(b2Color(0,0,0));
		frontWheel.setType(STATIC_BODY);
		rearWheel.setType(STATIC_BODY);
		chassis.setType(STATIC_BODY);
	}



	// Body
	b2Vec2 frontAnchor;
	b2Vec2 rearAnchor;
	sConcavePolygon chassis;
	sCircle frontWheel;
	sCircle rearWheel;
	sWheelJoint frontSuspension;
	sWheelJoint rearSuspension;

	// Brain
	sNeuralNet neuralNet;

protected:

	
	float payloadMass;
	float wheelMass;
	static const int progressTimeout = 350;
	int progressDelay;
	float progressPosition;
	float fitnessModifier;
	bool deferDeath;
	bool frontWheelContact;
	bool rearWheelContact;
	b2Vec2 lastLinearVelocity;
	float lastAngularVelocity;

	void setThrottle(float front, float rear)
	{
		//t *= payloadMass;
		//frontSuspension.setMaxMotorTorque(abs(front) * genome.getValue("frontMotorTorque"));
		//rearSuspension.setMaxMotorTorque(abs(rear) * genome.getValue("rearMotorTorque"));
		frontSuspension.setMaxMotorTorque(genome.getValue("frontMotorTorque"));
		rearSuspension.setMaxMotorTorque(genome.getValue("rearMotorTorque"));

		frontSuspension.setMotorSpeed(genome.getValue("frontMotorSpeed") * front);
		rearSuspension.setMotorSpeed(genome.getValue("rearMotorSpeed") * rear);
	}

	b2Color getAccelatorColor(float acc)
	{
		if(acc > 0){
			return b2Color(1.f - acc, 1, 1.f - acc);
		} else {
			return b2Color(1, 1.f + acc, 1.f + acc);
		}
	}

	// Gets change in velocity since last time the function was called
	b2Vec2 getLinearAcceleration()
	{
		b2Vec2 velocity = chassis.getLinearVelocity();
		b2Vec2 dv = velocity - lastLinearVelocity;

		// remove gravity force ?
		//dv -= m_world->timeStep * m_world->gravity;

		lastLinearVelocity = velocity;
		return dv;
	}

	// Gets change in angular velocity since last time the function was called
	float getAngularAcceleration()
	{
		float av = chassis.getAngularVelocity();
		float dav = av - lastAngularVelocity;
		lastAngularVelocity = av;
		return dav;
	}

	void onBeginContact(sContactPair contactPair)
	{
		if(contactPair.contains(&chassis)) {
			if(alive){
			//	fitnessModifier = 1.f;
			//	deferDeath = true;
			}
		} else if(contactPair.contains(&frontWheel)){
			frontWheelContact = true;
			//printf("front wheel touched \n");
		} else if(contactPair.contains(&rearWheel)){
			rearWheelContact = true;
		}
	}

	void onEndContact(sContactPair contactPair)
	{
		 if(contactPair.contains(&frontWheel)){
			 //printf("front wheel end contact \n");
			frontWheelContact = false;
		} else if(contactPair.contains(&rearWheel)){
			rearWheelContact = false;
		}
	}

};