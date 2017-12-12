#include "MyOctant.h"
using namespace Simplex; 

// assign values to member vars
MyOctant::MyOctant(uint a_depth, AABB * a_aabb, MyEntityManager * a_entityManager, MeshManager * a_meshManager)
{
	m_uDepth = a_depth;
	m_pAABB = a_aabb;
	m_pEntityManager = a_entityManager;
	m_pMeshManager = a_meshManager;
}

// delete the children from memory (orphan them??) 
// along with anything else that was a pointer
MyOctant::~MyOctant()
{
	/*delete[](m_pChildren);
	delete[](m_pEntityList); doesnt work on vectors
	delete(m_pAABB);
	m_pAABB = nullptr;*/
	
	// use SafeDelete in loop for array like
	for (uint i = 0; i < m_uNumChildren; i++) {
		SafeDelete(m_pChildren[i]);
	}

	for (uint j = 0; j < m_pEntityList.size(); ++j) {
		SafeDelete(m_pEntityList[j]);
	}

	SafeDelete(m_pAABB);
}

void MyOctant::Display()
{
	if (m_uDepth > 1) Display(1);
}

void MyOctant::Display(uint depth)
{
	m_pMeshManager->AddWireCubeToRenderList(glm::translate(m_pAABB->GetCenter()) * glm::scale(m_pAABB->GetSize()), C_YELLOW);
	for (uint i = 0; i < m_uNumChildren; ++i) {
		m_pChildren[i]->Display(depth + 1);
	}
}

AABB* MyOctant::Subdivide(uint index) {
	// 0 in front top right, and increasing ccw
	// codes: front/back, top/bottom, left/right (first initials)
	vector3 v3Min;
	vector3 v3Max;
	vector3 v3Half = m_pAABB->m_p3Max - m_pAABB->m_p3Min;

	v3Half.x = v3Half.x / 2;
	v3Half.y = v3Half.y / 2;
	v3Half.z = v3Half.z / 2;
	
	switch (index) {
	//ftr
	case 0:
		v3Min = vector3(
			m_pAABB->m_p3Min.x + v3Half.x,
			m_pAABB->m_p3Min.y + v3Half.y,
			m_pAABB->m_p3Min.z + v3Half.z
		);
		v3Max = m_pAABB->m_p3Max;
		break;

	//fbr
	case 1:
		v3Min = vector3(
			m_pAABB->m_p3Min.x + v3Half.x,
			m_pAABB->m_p3Min.y + v3Half.y,
			m_pAABB->m_p3Min.z + v3Half.z
		);
		v3Max = vector3(
			m_pAABB->m_p3Max.x,
			m_pAABB->m_p3Min.y + v3Half.y,
			m_pAABB->m_p3Max.z
		);
		break;

	//fbl
	case 2:
		v3Min = vector3(
			m_pAABB->m_p3Min.x,
			m_pAABB->m_p3Min.y,
			m_pAABB->m_p3Min.z + v3Half.z
		);
		v3Max = vector3(
			m_pAABB->m_p3Min.x + v3Half.x,
			m_pAABB->m_p3Min.y + v3Half.y,
			m_pAABB->m_p3Max.z
		);
		break;

	//ftl 
	case 3:
		v3Min = vector3(
			m_pAABB->m_p3Min.x,
			m_pAABB->m_p3Min.y + v3Half.y,
			m_pAABB->m_p3Min.z + v3Half.z
		);
		v3Max = vector3(
			m_pAABB->m_p3Min.x + v3Half.x,
			m_pAABB->m_p3Max.y,
			m_pAABB->m_p3Max.z
		);
		break;
		
		
	//btr
	case 4:
		v3Min = vector3(
			m_pAABB->m_p3Min.x + v3Half.x,
			m_pAABB->m_p3Min.y + v3Half.y,
			m_pAABB->m_p3Min.z 
		);
		v3Max = vector3(
			m_pAABB->m_p3Max.x,
			m_pAABB->m_p3Max.y,
			m_pAABB->m_p3Min.z + v3Half.z
		);
		break;

	//bbr
	case 5:
		v3Min = vector3(
			m_pAABB->m_p3Min.x + v3Half.x,
			m_pAABB->m_p3Min.y,
			m_pAABB->m_p3Min.z
		);
		v3Max = vector3(
			m_pAABB->m_p3Max.x,
			m_pAABB->m_p3Min.y + v3Half.y,
			m_pAABB->m_p3Min.z + v3Half.z
		);
		break;

	//bbl
	case 6:
		v3Min = m_pAABB->m_p3Min;
		v3Max = vector3(
			m_pAABB->m_p3Min.x + v3Half.x,
			m_pAABB->m_p3Min.y + v3Half.y,
			m_pAABB->m_p3Min.z + v3Half.z
		);
		break;

	//btl 
	case 7:
		v3Min = vector3(
			m_pAABB->m_p3Min.x,
			m_pAABB->m_p3Min.y + v3Half.y,
			m_pAABB->m_p3Min.z
		);
		v3Max = vector3(
			m_pAABB->m_p3Min.x + v3Half.x,
			m_pAABB->m_p3Max.y,
			m_pAABB->m_p3Min.z + v3Half.z
		);
		break;
	
	default:
		v3Min = v3Max = vector3(0.0f, 0.0f, 0.0f);		

	}

	return new AABB(v3Min, v3Max);
}

void MyOctant::MakeOctree() {
	m_pEntityList = std::vector<MyEntity*>();
	for (uint i = 0; i < m_pEntityManager->GetEntityCount(); ++i) {
		m_pEntityList.push_back(m_pEntityManager->GetEntity(i));
	}

	// make the first node
	if (m_uDepth > 1) {
		MakeOctree(1);
	}
}

void MyOctant::MakeOctree(uint depth)
{
	if (depth < m_uDepth) {
		m_uNumChildren = 8;
		for (uint i = 0; i < 8; ++i) {
			m_pChildren[i] = new MyOctant(
				m_uDepth, Subdivide(i), m_pEntityManager, m_pMeshManager
			);
			m_pChildren[i]->m_pParent = this; // set the parent for the children to this octant

			m_pChildren[i]->UpdateTree();

			// check if there is still too many entities
			if (m_pChildren[i]->m_pEntityList.size() > m_uMinEntities) {
				m_pChildren[i]->MakeOctree(depth + 1);
			}
		}
	}

	// no need to keep subdividing
	else {
		m_uNumChildren = 0;
		m_pParent = nullptr;
	}
}

void MyOctant::CheckCollisions()
{
	if (m_uDepth > 1) {
		CheckCollisions(1);
	}
}

void MyOctant::CheckCollisions(uint depth)
{
	if (m_pEntityList.size() > 0) {
		for (uint i = 0; i < m_pEntityList.size() - 1; ++i) {
			for (uint j = i + 1; j < m_pEntityList.size(); ++j) { // checks each object against each other with no repeats
				m_pEntityList[i]->IsColliding(m_pEntityList[j]);
			}
		}
	}

	// recurse and dheck deeper
	for (uint k = 0; k < m_uNumChildren; ++k) {
		m_pChildren[k]->CheckCollisions(depth + 1);
	}
}

void Simplex::MyOctant::UpdateTree()
{
	AABB currentAABB = AABB();
	for (uint i = 0; i < m_pParent->m_pEntityList.size(); ++i) {

		currentAABB = AABB(m_pParent->m_pEntityList[i]->GetRigidBody()->GetMinGlobal(),
			m_pParent->m_pEntityList[i]->GetRigidBody()->GetMaxGlobal());

		
		if (m_pAABB->Contains(&currentAABB)) {
			m_pEntityList.push_back(m_pParent->m_pEntityList[i]);
			m_pParent->m_pEntityList.erase(m_pParent->m_pEntityList.begin() + i);
			i--;
		}
	}
}
