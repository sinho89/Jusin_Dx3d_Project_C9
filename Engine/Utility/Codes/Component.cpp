#include "Component.h"

USING(Engine)

Engine::CComponent::CComponent(void)
{

}

Engine::CComponent::~CComponent(void)
{

}

void Engine::CComponent::Update_Component(const _float& fTimeDelta)
{

}

CComponent* Engine::CComponent::Clone(void)
{
	MSG_BOX("Component Clone Failed");
	return NULL;
}

