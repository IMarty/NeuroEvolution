#pragma once
#include <vector>
#include "sUniformBody.h"

using std::vector;

class sConcavePolygon : public sUniformBody
{

private:


public:

	sConcavePolygon(float32 x = 0, float32 y = 0, float32 angle = 0)
	{
		m_bodyDef = &s_bodyDef;
		setPosition(b2Vec2(x, y));
		setAngle(angle);

		vertexSplitBaseWeight = 2.f;
		segmentSplitBaseWeight = 1.0f;
		doubleSolveWeight = 2.0f;
		dotWeight = 1.0f;
	}

	void set(vector<b2Vec2> vertices)
	{
		m_vertices = vertices;
		finalizeShape();
	}

	void add(b2Vec2 vertex)
	{
		m_vertices.push_back(vertex);
	}
	void add(float32 x, float32 y)
	{
		add(b2Vec2(x, y));
	}
	void resetShape()
	{
		m_vertices.clear();
	}
	void finalizeShape()
	{
		m_parts.clear();

		simplify(m_vertices);

		simplifyCurvature(m_vertices);

		if(!validate(m_vertices)) return;

		split(m_vertices);
		
		int n_parts = m_parts.size();
		__fixtures.resize(n_parts);
		__shapes.resize(n_parts);
		m_fixtureDefs.resize(n_parts);


		for(int i = 0; i < n_parts; i++){
			//printf("--\n");
			//for(unsigned int j = 0; j < m_parts[i].size(); j++){
			//	printf("%f %f \n", m_parts[i][j].x, m_parts[i][j].y);
			//}
			__shapes[i].Set(&m_parts[i][0], m_parts[i].size());
			__fixtures[i].shape = &__shapes[i];
			m_fixtureDefs[i] = &__fixtures[i];
		}

	}


	// Split Weights
	float vertexSplitBaseWeight;
	float segmentSplitBaseWeight;
	float doubleSolveWeight;
	float dotWeight;
	float lengthWeight;

private:

	vector<b2FixtureDef> __fixtures;
	vector<b2PolygonShape> __shapes;

	bool lines_intersect(b2Vec2 const& p0, b2Vec2 const& p1, b2Vec2 const& p2, b2Vec2 const& p3, b2Vec2* i = 0) {
		b2Vec2 const s1 = p1 - p0;
		b2Vec2 const s2 = p3 - p2;

		b2Vec2 const u = p0 - p2;

		float32 const ip = 1.f / (-s2.x * s1.y + s1.x * s2.y);

		float32 const s = (-s1.y * u.x + s1.x * u.y) * ip;
		float32 const t = ( s2.x * u.y - s2.y * u.x) * ip;

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
			if (i) *i = p0 + (t * s1);
			return true;
		}

		return false;
	}

	bool has_intersection(const vector<b2Vec2> &vertices, int i0, int i1)
	{
		b2Vec2 v0 = vertices[i0 % vertices.size()];
		b2Vec2 v1 = vertices[i1 % vertices.size()];
		for(unsigned int i2 = 0; i2 < vertices.size(); i2++){
			int i3 = (i2 + 1) % vertices.size();
			if(i2 == i0 || i3 == i0 || i2 == i1 || i3 == i1) continue;
			b2Vec2 v2 = vertices[i2];
			b2Vec2 v3 = vertices[i3];
			if(lines_intersect(v0, v1, v2, v3))return true;
		}
		return false;
	}
	
	bool isConcave(const vector<b2Vec2> &vertices, int i)
	{
		return isConcave(vertices, i - 1, i, i + 1);
	}
	bool isConcave(const vector<b2Vec2> &vertices, int i0, int i1, int i2)
	{
		if(i0 < 0)i0 += vertices.size();
		b2Vec2 v0 = vertices[i0 % vertices.size()];
		b2Vec2 v1 = vertices[i1 % vertices.size()];
		b2Vec2 v2 = vertices[i2 % vertices.size()];
		return b2Cross(v1 - v0, v2 - v1) > 0;
	}
	bool isConcave(const b2Vec2 &v0, const b2Vec2 &v1, const b2Vec2 &v2)
	{
		return b2Cross(v1 - v0, v2 - v1) > 0;
	}

protected:


	void simplify(vector<b2Vec2> &vertices)
	{
		for(unsigned int i = 0; i < vertices.size(); i++){
			if(vertices.size() < 3)return;

			b2Vec2 v0 = vertices[i ? i - 1 : vertices.size() - 1];
			b2Vec2 v1 = vertices[i];
			b2Vec2 v2 = vertices[(i + 1) % vertices.size()];

			if(b2DistanceSquared(v1, v2) < 0.5f * b2_linearSlop){
				vertices.erase(vertices.begin() + i);
				i--;
				break;
			}
		}
	}


	void simplifyCurvature(vector<b2Vec2> &vertices)
	{
		for(unsigned int i = 0; i < vertices.size(); i++){

			b2Vec2 v0 = vertices[i > 0 ? i - 1 : vertices.size() - 1];
			b2Vec2 v1 = vertices[i];
			b2Vec2 v2 = vertices[(i + 1) % vertices.size()];

			b2Vec2 s0 = v1 - v0;
			b2Vec2 s1 = v2 - v1;
			s0.Normalize();
			s1.Normalize();
			//printf("b2dot = %f \n", i, b2Dot(s0, s1));

			float dot = b2Dot(s0,s1);

			if(!isConcave(v0,v1,v2)){
				dot = float(pow(dot, 0.5));
			}

			if(dot > 0.99995f){
				vertices.erase(vertices.begin() + i);
				i--;
			}

		}
	}

	bool validate(vector<b2Vec2> &vertices)
	{
		int nv = vertices.size();
		if(nv < 3){
			printf("Splitter Error: Not enough vertices \n");
			return false;
		}
		for(int i = 0; i < nv-2; i++){
			for(int j = i + 2; j < nv-1; j++){
				if(lines_intersect(vertices[i], vertices[i+1], vertices[j], vertices[j+1])){
					printf("Splitter Error: intersecting lines \n");
					return false;
				}
			}
		}
		float32 a = 0;
		for(int i = 0; i < nv; i++){
			a += (vertices[i].x + vertices[(i + 1) % nv].x) * (vertices[i].y - vertices[(i + 1) % nv].y);
		}
		if(a < 0){
			//printf("reversing winding \n", a);
			int i = 0;
			int j = nv-1;
			while(j > i){
				b2Vec2 t = vertices[i];
				vertices[i] = vertices[j];
				vertices[j] = t;
				i++; j--;
			}
		}
		return true;

	}



	//m_fixtureDefs.push_back(&s_fixtureDef);
	//fixtureDef.shape = &shape;
	void split(vector<b2Vec2> &vertices, int n_rec = 0, bool forceSplit = false)
	{
		if(vertices.size() < 3){
			printf("split() not enough vertices : %i\n", vertices.size());
		}
		if(n_rec > 3000){
			printf("split() overflow");
			return;
		}
		int n_verts = vertices.size(); 
		for(int i = 1; i <= n_verts; i++){

			 b2Vec2 v0 = vertices[(i - 1) % n_verts];
			 b2Vec2 v1 = vertices[(i + 0) % n_verts];
			 b2Vec2 v2 = vertices[(i + 1) % n_verts];

			 // check if concave
			//if(b2Cross(v1 - v0, v2 - v1) > 0){
			if(isConcave(vertices, i) || forceSplit){

				// Get mid normal
				b2Vec2 dv1 = v0 - v1;
				b2Vec2 dv2 = v2 - v1;
				dv1.Normalize();
				dv2.Normalize();
				b2Vec2 split_normal = forceSplit ? dv1 + dv2 :- dv1 - dv2;
				split_normal.Normalize();

				// TODO: Get a point split candidate

				int splitVertex_index = -1;
				float best_split_score = 0;
				bool split_found = false;
				
				float32 best_dot = 0.f;

				for(int j = 2; j < n_verts - 1; j++){
				
					int i1 = (i + j) % n_verts;
					b2Vec2 v3 = vertices[i1];
					if(!has_intersection(vertices, i, i1)){
						if(!isConcave(v0,v1,v3) && !isConcave(v3,v1,v2)){

							float split_score = vertexSplitBaseWeight;

							b2Vec2 normal(v3 - v1);
							float length = normal.Normalize();

							float32 dot = b2Dot(normal, split_normal);

							if(isConcave(vertices, i1)){
								if(!isConcave(vertices, i, i1, i1 + 1) && 
							       !isConcave(vertices, i1 - 1, i1, i)){
									split_score *= doubleSolveWeight;
								}
							}

							//split_score /= length;
							float lenW = 1.f / (1.f + length);
							split_score *= (lenW * lengthWeight) + (1.f - lengthWeight);

							split_score *= (dot * dotWeight) + (1.f - dotWeight);

							if(split_score > best_split_score){
								best_split_score = split_score;
								splitVertex_index = i1;

							}
						} else {
							//printf("result not convex\n");
						}
					} else {
						//printf("\nhas intersection\n");
					}
				}



				
				b2Vec2 v1n = v1 + (10000.f * split_normal);

				// Find closest/first intersection
				int first_i = 0;
				float32 first_len = 1000.f;
				b2Vec2 first_p;
				bool found = false;
				for(int j = 1; j < n_verts-1; j++){

					int i1 = (i + j) % n_verts;
					int i2 = (i + j + 1) % n_verts;
					b2Vec2 o1 = vertices[i1];
					b2Vec2 o2 = vertices[i2];
					b2Vec2 ip; // intersection point

					if(lines_intersect(o1, o2, v1, v1n, &ip)){
						float32 len = (ip - v1).Length();
						if(len < first_len){
							first_len = len;
							first_i = i1;
							first_p = ip;
							found = true;
						}
					}
				}

				if(!found){
					printf("Error, no intersection found. \n");
					return;
				}

				float32 score = segmentSplitBaseWeight;
				float lenW = 1.f / (1.f + first_len);
				score *= (lenW * lengthWeight) + (1.f - lengthWeight);

				if(score > best_split_score){
					splitVertex_index = -1;
					//best_split_score = score;
				}
				//printf("\nbest v split score = %f, vi = %i \n", score, splitVertex_index);
				//printf("best s split score = %f, vi = %i \n", best_split_score, splitVertex_index);
				vector<b2Vec2> r1, r2;
				if(splitVertex_index == -1){
					vector<b2Vec2> *rcur = &r1;
					for(int j = 0; j <= n_verts; j++){
						int k = (i + j) % n_verts;
						rcur->push_back(vertices[k]);
						if(k == first_i){
							rcur->push_back(first_p);
							rcur = &r2;
							rcur->push_back(first_p);
						}
					}
				} else {
					vector<b2Vec2> *rcur = &r1;
					for(int j = 0; j <= n_verts; j++){
						int k = (i + j) % n_verts;
						rcur->push_back(vertices[k]);
						if(k == splitVertex_index){
							rcur = &r2;
							rcur->push_back(vertices[k]);
						}
					}
				}

				// Recursivly split result
				split(r2, n_rec+1);
				split(r1, n_rec+1);

				return;
			 }
		}
		if(vertices.size() > b2_maxPolygonVertices){
			//split(vertices, n_rec+1, true);
			convexSplit(vertices);
		} else {
			simplify(vertices);
			if(vertices.size() < 3){
				//printf("Degenerate shape, removing\n");
				return;
			}
			m_parts.push_back(vertices);
		}
	}


	b2Vec2 splitNormal(const b2Vec2 &v0, const b2Vec2 &v1, const b2Vec2 &v2)
	{
		// Get mid normal
		b2Vec2 dv1 = v0 - v1;
		b2Vec2 dv2 = v2 - v1;
		dv1.Normalize();
		dv2.Normalize();
		b2Vec2 split_normal = dv1 + dv2;
		split_normal.Normalize();
		return split_normal;
	}

	void convexSplit(vector<b2Vec2> &vertices)
	{
		int n_verts = vertices.size(); 
		float bestScore = 0;

		int offset = vertices.size() / 2 - 1;
		if(offset > 7)offset = 7;

		int bestVetrex1 = 0, bestVetrex2 = 0;

		for(int i = 1; i <= n_verts; i++){

			b2Vec2 v0  = vertices[(i + 0) % n_verts];
			b2Vec2 v0a = vertices[(i + 1) % n_verts];
			b2Vec2 v0b = vertices[(i - 1) % n_verts];

			b2Vec2 v1 = vertices[(i + offset) % n_verts];
			b2Vec2 v1a = vertices[(i + offset - 1) % n_verts];
			b2Vec2 v1b = vertices[(i + offset + 1) % n_verts];

			// Get mid normal
			b2Vec2 split_normal1 = splitNormal(v0b, v0, v0a);
			b2Vec2 split_normal2 = splitNormal(v1b, v1, v1a);

			b2Vec2 split_vector = v0 - v1;
			split_vector.Normalize();

			float32 dot1 = b2Dot(split_normal1, -split_vector);
			float32 dot2 = b2Dot(split_normal2, split_vector);

			float score = dot1 + dot2;

			if(score > bestScore){
				bestScore = score;
				bestVetrex1 = (i + 0) % n_verts;
				bestVetrex2 = (i + offset) % n_verts;
			}

		}


		vector<b2Vec2> r1, r2;
		vector<b2Vec2> *rcur = &r1;
		for(int j = 0; j <= n_verts; j++){
			int k = (bestVetrex1 + j) % n_verts;
			rcur->push_back(vertices[k]);
			if(k == bestVetrex2){
				rcur = &r2;
				rcur->push_back(vertices[k]);
			}
		}
		m_parts.push_back(r1);

		if(r2.size() > b2_maxPolygonVertices){
			convexSplit(r2);
		} else {
			m_parts.push_back(r2);
		}


	}


	//void addToWorld(sWorld &world)
	//{
	//	sUniformBody::addToWorld(world);
	//}
	vector<vector<b2Vec2>> m_parts;
	vector<b2Vec2> m_vertices;

	b2BodyDef s_bodyDef;
};