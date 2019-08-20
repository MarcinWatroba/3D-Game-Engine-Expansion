#include <Engine\Component\Collider.h>

Collider::Collider()
{}
Collider::~Collider()
{}
Collider::Collider(const Collider &p_NewComp_In)
{}
std::string Collider::get_Type()
{
	return "Collider";
}