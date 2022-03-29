#pragma once

#include <typeindex>
#include <vector>

#include "includegl.h"

#include "vertex.h"


// Repräsentiert eine Liste von Scheitelpunkten eines 3D-Modells zusammen mit der Anordnung der Daten, welche
// sich im Arbeitsspeicher der Grafikkarte befinden
class Vertexbuffer {

	uint32_t va, id;
	std::vector<Vertex> data;

public:
	Vertexbuffer();
	Vertexbuffer(std::vector<Vertex> data, int mode = GL_STATIC_DRAW);
	~Vertexbuffer();

	void init(std::vector<Vertex> data, int mode = GL_STATIC_DRAW);

	void bind() const;
	void bindOnlyBuffer() const;

	inline uint32_t getID() const { return id; }
	inline uint32_t getVertexarray() const { return va; }
	inline int getCount() const { return data.size(); }
};

// Repräsentiert eine Liste von Indexen der Scheitelpunkte, welche zusammen ein Dreieck bilden.
class Indexbuffer {

	uint32_t id;
	std::vector<uint32_t> data;

public:
	Indexbuffer();
	Indexbuffer(std::vector<uint32_t> data, int mode = GL_STATIC_DRAW);
	~Indexbuffer();

	void init(std::vector<uint32_t> data, int mode = GL_STATIC_DRAW);

	void bind() const;

	inline uint32_t getID() const { return id; }
	inline int getCount() const { return data.size(); }
};
