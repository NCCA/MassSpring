#include "RK4Spring.h"
#include <QDebug>

RK4Spring::RK4Spring( ngl::Vec3 _a, ngl::Vec3 _b, float _length, float _k, float _damping	)
{
	// ctor sets the initial values
	m_a=_a;
	m_b=_b;
	m_initialA=_a;
	m_initialB=_b;

	m_length=_length;
	m_k=_k;
	m_damping=_damping;
	m_state.m_position=m_b-m_a;

	ngl::Vec3 distance=m_b-m_a;
	float length=distance.length();
	m_state.m_velocity.set(0.0f,0.0f,0.0f);

	m_timestep=0.1f;
	m_t=0.0f;
	m_aFixed=false;
	m_bFixed=false;
}

void RK4Spring::reset()
{
	// set the timestep to 0 and re-set the position values
	m_t=0;
	m_a=m_initialA;
	m_b=m_initialB;
	m_state.m_position=m_b-m_a;
	m_state.m_velocity.set( 0.0f,0.0f,0.0f);


}
void RK4Spring::update()
{
	// run the RK4 integrator to get the new force values
	integrate(m_t, m_timestep);
	// update the timestep for the next time
	m_t += m_timestep;
	// now  apply to spring points (if they are not fixed)
	if( m_aFixed !=true)
	{
		m_a-=getState().m_velocity;
	}
	if(m_bFixed !=true)
	{
		m_b+=getState().m_velocity;
	}

	//qDebug()<<"Time step "<<m_t;
	//qDebug()<<"k="<<m_k<<" length= "<<m_length<<" damping "<<m_damping;
}


ngl::Vec3 RK4Spring::motionFunction(	const State &_state,float _t)
{
  // in the case of the spring we don't need t mark
  // it as unused in this case
  Q_UNUSED(_t);
  /// F = -k(|x|-d)(x/|x|) - bv where
	/// k is the spring constant
	/// |x| is the distance between the two spring points
	/// d is the distance of seperation
	/// b is the coefficient of damping larger b increases the damping force
	/// v is the relative velocity between the spring points
	ngl::Vec3 distance=m_b-m_a;
	float length=distance.length();
	return -m_k*(length-m_length)*(distance/length)-m_damping*_state.m_velocity;

}
