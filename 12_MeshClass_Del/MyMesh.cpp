#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}

void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();

}

void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);

}

void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded) return;
	if (m_uVertexCount < 3) return;

	for (uint i = 0; i < m_uVertexCount; ++i) {
		m_lVertex.push_back(m_lVertexPos[i]);
		m_lVertex.push_back(m_lVertexCol[i]);
	}


	CompleteMesh(C_GREEN_DARK);

	glGenVertexArrays(1, &m_VAO); // function, needs the reference (argument as return type)  
	glGenBuffers(1, &m_VBO);

	// set current VAO as active (VBO would work because both are same return type, but woudl break)
	glBindVertexArray(m_VAO);
	
	// bind and specify type of buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO); 

	// how to use info -> (type, number of/size of * 2 for dimensions * how big the container is, how to use info (dynamic or static [values will not change]) 
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);

	// attrib layer for position in shader
	glEnableVertexAttribArray(0);
	// start position, num entries, type, normalize, how far apart each entry is 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// attrib layer for color in shader
	glEnableVertexAttribArray(1);
	// start position, num entries, type, normalize, how far apart each entry is 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*) (1*sizeof(vector3)));
	// flag that says already binded
	m_bBinded = true;

	// disable VA
	glBindVertexArray(0);
}

void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	//if nothing has been bound
	if (m_bBinded == false) return;

	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader);

	//make active
	glBindVertexArray(m_VAO);

	// does shader have variable named MVP? gimme that loc then
	GLuint MVP = glGetUniformLocation(nShader, "MVP");

	// reverse order b/c column major (multiply right to left)
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;

	//send to gfx card
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));

	// how to render
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// how to draw/how many to draw
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
}
