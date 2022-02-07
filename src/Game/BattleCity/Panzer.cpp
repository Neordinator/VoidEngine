#include "Panzer.h"

Panzer::Panzer(const float velocity, const glm::vec2& position, const glm::vec2& size, const float layer)
	: IGameObject(position, size, 0.f, layer), m_eOrientation(EOrientation::Top)
	, m_pSprite_top(ResourceManager::getSprite("yellowPanzer1_top"))
	, m_pSprite_bottom(ResourceManager::getSprite("yellowPanzer1_bottom"))
	, m_pSprite_left(ResourceManager::getSprite("yellowPanzer1_left"))
	, m_pSprite_right(ResourceManager::getSprite("yellowPanzer1_right"))
	, m_spriteAnimator_top(m_pSprite_top), m_spriteAnimator_bottom(m_pSprite_bottom), m_spriteAnimator_left(m_pSprite_left), m_spriteAnimator_right(m_pSprite_right)
	, m_move(false), m_velocity(velocity), m_moveOffset(glm::vec2(0.f, 1.f))
{
}

void Panzer::render() const
{
	switch (m_eOrientation)
	{
	case Panzer::EOrientation::Top:
		m_pSprite_top->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_top.getCurrentFrame());
		break;
	case Panzer::EOrientation::Bottom:
		m_pSprite_bottom->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_bottom.getCurrentFrame());
		break;
	case Panzer::EOrientation::Left:
		m_pSprite_left->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_left.getCurrentFrame());
		break;
	case Panzer::EOrientation::Right:
		m_pSprite_right->render(m_position, m_size, m_rotation, m_layer, m_spriteAnimator_right.getCurrentFrame());
		break;
	default:
		break;
	}
}

void Panzer::setOrientation(const EOrientation eOrientation)
{
	if (m_eOrientation == eOrientation)
	{
		return;
	}
	m_eOrientation = eOrientation;
	switch (m_eOrientation)
	{
	case Panzer::EOrientation::Top:
		m_moveOffset.x = 0.f;
		m_moveOffset.y = 1.f;
		break;
	case Panzer::EOrientation::Bottom:
		m_moveOffset.x = 0.f;
		m_moveOffset.y = -1.f;
		break;
	case Panzer::EOrientation::Left:
		m_moveOffset.x = -1.f;
		m_moveOffset.y = 0.f;
		break;
	case Panzer::EOrientation::Right:
		m_moveOffset.x = 1.f;
		m_moveOffset.y = 0.f;
		break;
	default:
		break;
	}
}

void Panzer::move(const bool move)
{
	m_move = move;
}

void Panzer::update(const uint64_t delta)
{
	if (m_move)
	{
		m_position += delta * m_velocity * m_moveOffset;
		switch (m_eOrientation)
		{
		case Panzer::EOrientation::Top:
			m_spriteAnimator_top.update(delta);
			break;
		case Panzer::EOrientation::Bottom:
			m_spriteAnimator_bottom.update(delta);
			break;
		case Panzer::EOrientation::Left:
			m_spriteAnimator_left.update(delta);
			break;
		case Panzer::EOrientation::Right:
			m_spriteAnimator_right.update(delta);
			break;
		}
	}
}
