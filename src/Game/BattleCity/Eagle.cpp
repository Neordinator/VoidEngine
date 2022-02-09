#include "Eagle.h"

class IGameObject;

namespace Render
{
	class Sprite;
}
Eagle::Eagle( const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: IGameObject(position, size, rotation, layer)
	, m_sprite({ ResourceManager::getSprite("eagle"), ResourceManager::getSprite("eagle_dead") })
	, m_eCurrentState(Eagle::EEagleState::Alive)
{
	m_colliders.emplace_back(glm::vec2(0), m_size);
}

void Eagle::render() const
{
	m_sprite[static_cast<size_t>(m_eCurrentState)]->render(m_position, m_size , m_rotation, m_layer);
}

void Eagle::update(const double delta)
{
}
