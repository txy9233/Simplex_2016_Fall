#ifndef __MYOCTANT_H_
#define __MYOCTANT_H_

#include "MyEntityManager.h"
// I tried using the ideas found at https://www.gamedev.net/articles/programming/general-and-gameplay-programming/introduction-to-octrees-r3529/

namespace Simplex
{
	// AABB struct that will help detect when RB's are inside an octant
	struct AABB
	{
		vector3 m_v3Min;
		vector3 m_v3Max;

		// constructors
		AABB() 
		{
			m_v3Min = vector3(0, 0, 0);
			m_v3Max = vector3(0, 0, 0);
		}

		AABB(vector3 min, vector3 max) 
		{
			m_v3Min = min;
			m_v3Max = max;
		}

		// nothing to delete
		~AABB() { }

		
		bool Contains(AABB* other) 
		{
			return (m_v3Min.x < other->m_v3Min.x &&
				m_v3Min.y < other->m_v3Min.y &&
				m_v3Min.z < other->m_v3Min.z &&
				m_v3Max.x > other->m_v3Max.x &&
				m_v3Max.y > other->m_v3Max.y &&
				m_v3Max.z > other->m_v3Max.z);
		}

		// return the center of the AABB
		vector3 GetCenter()	
		{
			return vector3(m_v3Min.x + (m_v3Max.x - m_v3Min.x) / 2,
				m_v3Min.y + (m_v3Max.y - m_v3Min.y) / 2,
				m_v3Min.z + (m_v3Max.z - m_v3Min.z) / 2);
		}

		vector3 GetSize() 
		{
			return vector3((m_v3Max.x - m_v3Min.x) * 1,
				(m_v3Max.y - m_v3Min.y) * 1,
				(m_v3Max.z - m_v3Min.z) * 1);
		}
	};

	class  MyOctant
	{
	private:
		uint m_uDepth;
		uint m_uMinEntities = 5;		
		uint m_uNumChildren = 0; // start with 0 and de/increment as needed
		
		AABB* m_pAABB = nullptr; // aabb data for this octant
		
		MyOctant* m_pParent = nullptr; // this will eventually point to the parent octant/node
		MyOctant* m_pChildren[8]; // array that holds the 8 children of this octant/node
		
		MyEntityManager* m_pEntityManager = nullptr; 
		
		MeshManager* m_pMeshManager = nullptr;

		std::vector<MyEntity*> m_pEntityList;

		AABB* Subdivide(uint index); // subdivide into desired index
		void UpdateTree(); // updates the octree by moving Entities into deeper subdivision

		// recursive functions - private because... you know.
		void Display(uint depth);
		void MakeOctree(uint depth);
		void CheckCollisions(uint depth);

	public:
		MyOctant(uint depth, AABB* aabb, MyEntityManager* a_entityMngr, MeshManager* a_mshMngr);
		~MyOctant();

		// public versions that will call the recursive functions 
		void Display(); 		
		void MakeOctree();
		void CheckCollisions();
	};
};


#endif // !__MYOCTANT_H_