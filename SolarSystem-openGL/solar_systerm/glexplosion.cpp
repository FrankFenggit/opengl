#include<ctime>
#include "glexplosion.h"

//*****************************************************************************/
glExplosion::glExplosion(int numParticles, vec3 origin, float spread, GLuint texture)
	: m_texture(texture), m_spread(spread), glParticleSystem(numParticles, origin)
{
	srand(time(NULL));
	glParticleSystem::InitializeSystem();
	Emit(numParticles);
}

void glExplosion::InitializeParticle(int index)//��ը��ʼ��
{
	m_particleList[index].m_pos.x = m_origin.x + FRAND * m_spread;
	m_particleList[index].m_pos.y = m_origin.y + FRAND * m_spread;
	m_particleList[index].m_pos.z = m_origin.z + FRAND * m_spread;

	m_particleList[index].m_size = PARTICLE_SIZE + FRAND * SIZE_VARIATION;

	m_particleList[index].m_velocity.x = PARTICLE_VELOCITY.x + FRAND * VELOCITY_VARIATION.x;
	m_particleList[index].m_velocity.y = PARTICLE_VELOCITY.y + FRAND * VELOCITY_VARIATION.y;
	m_particleList[index].m_velocity.z = PARTICLE_VELOCITY.z + FRAND * VELOCITY_VARIATION.z;


	m_particleList[index].m_acceleration = PARTICLE_ACCELERATION;
	m_particleList[index].m_color[0] = 1.0;
	m_particleList[index].m_color[1] = 0.5f + FRAND * 0.5f;
	m_particleList[index].m_color[2] = 0.01f;
	m_particleList[index].m_color[3] = 1.0;
	m_particleList[index].m_energy = 1.5f + FRAND / 2.0f;
	m_particleList[index].m_colorDelta[0] = 0.0;
	m_particleList[index].m_colorDelta[1] = -(m_particleList[index].m_color[1] / 2.0f) / m_particleList[index].m_energy;
	m_particleList[index].m_colorDelta[2] = 0.0;
	m_particleList[index].m_colorDelta[3] = -1.0f / m_particleList[index].m_energy;
	m_particleList[index].m_sizeDelta = -m_particleList[index].m_size / m_particleList[index].m_energy;
}
void glExplosion::Update(float elapsedTime)//�����ը����
{
	for (int i = 0; i < m_numParticles;)
	{
		//����λ��s=vt;
		m_particleList[i].m_pos = m_particleList[i].m_pos + m_particleList[i].m_velocity * elapsedTime;
		//�����ٶ�v=at;
		m_particleList[i].m_velocity = m_particleList[i].m_velocity + m_particleList[i].m_acceleration * elapsedTime;
		//��������ʱ������
		m_particleList[i].m_energy -= elapsedTime;

		//size�Լ���ɫ����ʱ��任
		m_particleList[i].m_size += m_particleList[i].m_sizeDelta * elapsedTime;
		m_particleList[i].m_color[3] += m_particleList[i].m_colorDelta[3] * elapsedTime;
		m_particleList[i].m_color[1] += m_particleList[i].m_colorDelta[1] * elapsedTime;

		//�����ǰ���ӵ�����<=0��˵��û����
		//�����һ�����ӷ��뵱ǰλ��
		if (m_particleList[i].m_energy <= 0.0)
		{
			m_particleList[i] = m_particleList[--m_numParticles];
		}
		else
		{
			++i;
		}
	}
}


void glExplosion::Render()//��ը����
{
	glPushAttrib(GL_CURRENT_BIT);//����������ɫ���� glPopAttrib();//�ָ�ǰһ����

	float viewMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, viewMatrix);
	vec3 right(viewMatrix[0], viewMatrix[4], viewMatrix[8]);
	vec3 up(viewMatrix[1], viewMatrix[5], viewMatrix[9]);

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBegin(GL_QUADS);
	for (int i = 0; i < m_numParticles; ++i)
	{
		GLfloat size = m_particleList[i].m_size / 3;
		vec3 pos = m_particleList[i].m_pos;
		glColor4fv(m_particleList[i].m_color);

		glTexCoord2f(0.0, 0.0); glVertex3fv((pos + (right + up) * -size).v);
		glTexCoord2f(1.0, 0.0); glVertex3fv((pos + (right - up) * size).v);
		glTexCoord2f(1.0, 1.0); glVertex3fv((pos + (right + up) * size).v);
		glTexCoord2f(0.0, 1.0); glVertex3fv((pos + (up - right) * size).v);
	}
	glEnd();
	//glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glPopAttrib();//�ָ�ǰһ����
}