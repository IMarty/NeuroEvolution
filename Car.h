#pragma once
#include "sPhysics\sPhysics.h"
#include "sPhysics\sWheelJoint.h"

using std::vector;

class Car : public sContainer
{

public:

	Car()
	{

		m_wheel1.setRadius(0.5);
		m_wheel1.setPosition(b2Vec2(-1.5, 0.5));
		m_wheel1.setFriction(1);
		m_wheel1.setRestitution(0.0f);
		add(&m_wheel1);

		m_wheel2.copy(m_wheel1);
		m_wheel2.setPosition(b2Vec2(1.5, 0.5));
		m_wheel2.setFriction(1);
		m_wheel2.setRestitution(0.0f);
		add(&m_wheel2);

		m_car.add(2.f, 0.f);
		m_car.add(2.f, -0.5f);
		m_car.add(1.5f, -0.6f);
		m_car.add(1.f, -1.f);
		m_car.add(-1.f, -1.f);
		m_car.add(-1.5f, -0.6f);
		m_car.add(-2.f, -0.5f);
		m_car.add(-2.f, 0.f);
		m_car.finalizeShape();
		m_car.setDensity(1);
		add(&m_car);


		b2Vec2 anchor1(-1.5f, 0.f);
		b2Vec2 anchor2( 1.5f, 0.f);

		m_wheelJoint1.setBodies(&m_car, &m_wheel1);
		m_wheelJoint1.setAnchorA(m_wheel1.getPosition());
		m_wheelJoint1.setAnchorB(anchor1);
		m_wheelJoint1.setAxis(anchor1 - m_wheel1.getPosition());
		m_wheelJoint1.setEnableMotor(true);
		m_wheelJoint1.setMotorSpeed(0);
		m_wheelJoint1.setMaxMotorTorque(5);
		m_wheelJoint1.setFrequencyHz(3.5f);
		m_wheelJoint1.setDampingRatio(0.9f);
		add(&m_wheelJoint1);


		m_wheelJoint2.copy(m_wheelJoint1);
		m_wheelJoint2.setBodyB(&m_wheel2);
		m_wheelJoint2.setAnchorB(anchor2);
		m_wheelJoint2.setAnchorA(m_wheel2.getPosition());
		m_wheelJoint2.setAxis(anchor2 - m_wheel2.getPosition());
		/*
		m_wheelJoint2.setEnableMotor(true);
		m_wheelJoint1.setMotorSpeed(0);		
		m_wheelJoint2.setMaxMotorTorque(5);
		m_wheelJoint2.setFrequencyHz(3.5f);
		m_wheelJoint2.setDampingRatio(0.9f);
		*/
		add(&m_wheelJoint2);

	}


	void setAccelerator(float t)
	{
		m_wheelJoint1.setMaxMotorTorque(abs(t) * 5);
		m_wheelJoint2.setMaxMotorTorque(abs(t) * 5);
		m_wheelJoint1.setMotorSpeed(100 * t);
		m_wheelJoint2.setMotorSpeed(100 * t);
	}


	sConcavePolygon m_car;
	sCircle m_wheel1;
	sCircle m_wheel2;
	sWheelJoint m_wheelJoint1;
	sWheelJoint m_wheelJoint2;



};