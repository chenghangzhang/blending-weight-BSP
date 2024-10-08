#include "mPch.h"
#include "RemeshOperate.h"

RemshOperate::RemshOperate(Mesh& p_Mesh, MeshIntersectResult& p_RecordMap, RemeshNewVertexRecord& p_ReturnNewVertex, const Tolerance& p_Tolerence/*= Tolerance()*/ ) :
    m_OriginalMesh(p_Mesh),m_RecordMap(p_RecordMap), m_ReturnNewVertex(p_ReturnNewVertex), m_Tolerance(p_Tolerence)
{

}

void RemshOperate::Initialize()
{
    assert(false == m_IsInitialize);

    cout << "newMesh=========================" << endl;
    for (Mesh::FaceIter it = m_OriginalMesh.faces_sbegin(); it != m_OriginalMesh.faces_end(); ++it) {

        //const Mesh::Face& triangle = m_OriginalMesh.face(*it);
        FaceId triangle = (*it).idx();
        //triangle = (*it).idx();
        //如果有记录返回记录，没有记录返回nullptr
        //在记录中没有triangle信息，属于一类面（暂时如此，后续可能会涉及到虽然有记录，但是也属于一类面情况）
        MeshTriangleIntersectRecord* triangleMshIntersectRecord = m_RecordMap.At(triangle);

        if (nullptr != triangleMshIntersectRecord) {

            m_ProcessingTriangles.insert(make_pair((*it).idx(), triangleMshIntersectRecord));
        }
    }

    m_WaitingProcessingTriangles = m_ProcessingTriangles;
    //size_t triangleCount = m_OriginalMesh.GetFacetCount();
    ////遍历原始Mesh中每个三角形，并进行处理
    //for (size_t i = 0; i < triangleCount; i++)
    //{

    //    const MeshTriangleHandle& triangle = m_OriginalMesh.FacetAt(i);

    //    //如果有记录返回记录，没有记录返回nullptr
    //    //在记录中没有triangle信息，属于一类面（暂时如此，后续可能会涉及到虽然有记录，但是也属于一类面情况）
    //    const MeshTriangleIntersectRecord* triangleMshIntersectRecord = m_RecordMap.At(&triangle);

    //    if (nullptr != triangleMshIntersectRecord) {

    //        m_ProcessingTriangles.insert(const_cast<MeshTriangleHandle*>(&triangle));
    //    }

    //}

    m_IsInitialize = true;
}

void RemshOperate::InitializeSimplifyMesh()
{
    //将原始网格中所有的顶点处理为(MeshIntersectOriginalVertex)MeshIntersectVertex*
    //三角形先不急添加
    // 
    // 同时将路径中所有MeshIntersectVertex*也添加进网格来
    //生成一个MeshIntersectVertex*点云

    //m_SimplifyMesh3d = new SimplifyMesh3d();

}


bool RemshOperate::GenerateRemesh(FaceId p_Triangle,const MeshTriangleIntersectRecord* p_Record)
{
    assert(p_Triangle <= m_OriginalMesh.n_faces() );

    //cout << "new record=================================" << endl;

    //unordered_set<MeshTriangle*> p_NewTriangles;

    TriangleArgs p_TriArg;

    if (!p_Record->hasChanged) {
        p_TriArg.m_NewTriangles.insert(p_Triangle);
        p_TriArg.m_CheckingEdges.clear();   
    }
    else {
        p_TriArg.m_NewTriangles.insert(p_Record->m_NewTriangles.begin(),p_Record->m_NewTriangles.end());
        p_TriArg.m_CheckingEdges.clear();
    }


    //Mesh::FaceVertexCCWIter fvccwIter = m_OriginalMesh.fv_ccwbegin(m_OriginalMesh.face_handle(p_Triangle));

    //Point3d p0(m_OriginalMesh.point(*fvccwIter));
    //Point3d p1(m_OriginalMesh.point(*++fvccwIter));
    //Point3d p2(m_OriginalMesh.point(*++fvccwIter));

    //p_TriArg.m_Edges.push_back(LineSegment3d(p0, p1));
    //p_TriArg.m_Edges.push_back(LineSegment3d(p1, p2));
    //p_TriArg.m_Edges.push_back(LineSegment3d(p2, p0));     

    Tolerance toler;
    toler.m_Linear = 1e-5;
    toler.m_Squared = 1e-5;

    
    //加点
    const vector<MeshIntersectPath*>& paths = p_Record->GetPaths();
    for (auto& path : paths) {

        //int addNum = 0;
        size_t sizeP = path->Size();
        for (int i = 0; i < sizeP;i++) {
            //addNum++;
            //if (addNum > 1) break;

            MeshIntersectVertex* miv = const_cast<MeshIntersectVertex*>(&((*path)[i]));

            //cout << "new Point Judge" << endl;
            //cout << "new Point Judge" << endl;
            //cout << "[" << (*(miv->m_Point3d))[0] << " " << (*(miv->m_Point3d))[1] << " " << (*(miv->m_Point3d))[2] << "]" << endl;
            //cout << endl;

            //{
            //    for (auto& newTri : p_TriArg.m_NewTriangles) {
            //        Mesh::FaceHandle fh = m_OriginalMesh.face_handle(newTri);

            //        auto triMath = TranslateToTriangle({fh,&m_OriginalMesh});
            //        cout << "[" << triMath.VertexAt(0)[0] << " " << triMath.VertexAt(0)[1] << " " << triMath.VertexAt(0)[2] << ";" << endl;
            //        cout << triMath.VertexAt(1)[0] << " " << triMath.VertexAt(1)[1] << " " << triMath.VertexAt(1)[2] << ";" << endl;
            //        cout << triMath.VertexAt(2)[0] << " " << triMath.VertexAt(2)[1] << " " << triMath.VertexAt(2)[2] << ";" << endl;
            //        cout << triMath.VertexAt(0)[0] << " " << triMath.VertexAt(0)[1] << " " << triMath.VertexAt(0)[2] << "]" << endl;
            //        cout << endl;

            //    }
            //        
            //}


            Point3dTriangle3dRelation rela;

            FaceId tri = -1;

            TestTriangle(p_TriArg);

            //cout << "new Point Test, Triangle Size: "<< p_TriArg.m_NewTriangles.size() << endl;
            for (auto& newTri : p_TriArg.m_NewTriangles) {
                TestTriangle(p_TriArg);

                Mesh::FaceHandle fh = m_OriginalMesh.face_handle(newTri);

                //bool te = m_OriginalMesh.is_valid_handle(fh);


                Mesh::FaceVertexCCWIter fvccwIter = m_OriginalMesh.fv_ccwbegin(fh);

                TestTriangle(p_TriArg);

                Triangle3d triMath(array<Point3d, 3>{
                    Point3d(m_OriginalMesh.point(*fvccwIter)),
                        Point3d(m_OriginalMesh.point(*(++fvccwIter))),
                        Point3d(m_OriginalMesh.point(*(++fvccwIter)))
                });

                TestTriangle(p_TriArg);

                //cout << "[" << triMath.VertexAt(0)[0] << " " << triMath.VertexAt(0)[1] << " " << triMath.VertexAt(0)[2] << ";" << endl;
                //cout << triMath.VertexAt(1)[0] << " " << triMath.VertexAt(1)[1] << " " << triMath.VertexAt(1)[2] << ";" << endl;
                //cout << triMath.VertexAt(2)[0] << " " << triMath.VertexAt(2)[1] << " " << triMath.VertexAt(2)[2] << ";" << endl;
                //cout << triMath.VertexAt(0)[0] << " " << triMath.VertexAt(0)[1] << " " << triMath.VertexAt(0)[2] << "]" << endl;
                //cout << endl;

                

                rela = DetectCoplanarPoint3dTriangle3dRelation(*(miv->m_Point3d), triMath, toler);

                switch(rela.m_Relation) {
                case Point3dTriangle3dRelation::Type::Out:
                    //cout << "Here Out" << endl;
                    break;
                case Point3dTriangle3dRelation::Type::OnVertex:
                    //cout << "Here OnVertex" << endl;
                        break;
                case Point3dTriangle3dRelation::Type::OnEdge:
                    //cout << "Here OnEdge" << endl;
                    break;
                case Point3dTriangle3dRelation::Type::In:
                    //cout << "Here In" << endl;
                    break;
                }


                if (rela.m_Relation != Point3dTriangle3dRelation::Type::Out) {
                    tri = newTri;
                    break;
                }
            }
                
                
            assert( tri >= 0 && tri <= m_OriginalMesh.n_faces());
            assert(rela.m_Relation != Point3dTriangle3dRelation::Type::Out);
                
            switch (rela.m_Relation) {
                case Point3dTriangle3dRelation::Type::OnVertex:
                {
                    //assert(false);
                    //交点在顶点上，暂不考虑
                    auto meshVertexPos = this->m_NewVertexs.find(miv);
                    if (meshVertexPos == m_NewVertexs.end()) {

                        Mesh::FaceVertexCCWIter fvccwIter = m_OriginalMesh.fv_ccwbegin(m_OriginalMesh.face_handle(tri));
                        for (int i = 0;i < rela.m_OnElmentIndex;i++) {
                            fvccwIter++;
                        }

                        this->m_NewVertexs[miv] = (*fvccwIter).idx();
                    }
                    break;
                }
                case Point3dTriangle3dRelation::Type::OnEdge:
                {
                    //Mesh::FaceHandle of = m_OriginalMesh.face_handle(tri);
                    //auto fhI = m_OriginalMesh.fh_ccwbegin(of);
                    //Mesh::HalfedgeHandle h1 = *(fhI++);
                    //Mesh::HalfedgeHandle h2 = *(fhI++);
                    //Mesh::HalfedgeHandle h3 = *(fhI++);


                    //bool t1 = m_OriginalMesh.is_boundary(h1);
                    //bool t2 = m_OriginalMesh.is_boundary(h2);
                    //bool t3 = m_OriginalMesh.is_boundary(h3);
                    //bool t4 = m_OriginalMesh.is_boundary(of);
                    
                    //if (m_ProcessedTvs.find(miv) != m_ProcessedTvs.end()) {
                    //    break;                       
                    //}

                    //m_ProcessedTvs.insert(miv);

                    //m_ProcessedTvs.find(dynamic_cast<MeshIntersectTerminalVertex*>(miv)->GetHostEdge().first.idx()) == ;


                    ProcessOnEdge(rela, tri, miv, p_TriArg);

                    TestTriangle(p_TriArg);
                    break;
                }                                 
                case Point3dTriangle3dRelation::Type::In:
                    ProcessIn(rela, tri, miv, p_TriArg);

                    TestTriangle(p_TriArg);
                    break;
                
                default:

                    assert(false);
                    break;
            }
        }
        
        
    }

    TestTriangle(p_TriArg);
        


    //int i = 0;
    //添加约束边       
    for (auto& path : paths) {      
        vector<PathSegment*>& pathSegs = path->GetPathSegments();

        assert(pathSegs.size() > 0);

        for (auto& pathSeg : pathSegs) {

            ProcessConstrainedEdge(pathSeg, p_TriArg);
                       
        }

        //i++;
    }


    //在系统足够稳定后，是否应该删去
    //核验CDT特性
    //while (!CheckingCDT(p_TriArg));
    //assert(CheckingCDT(p_TriArg));

 
    return true;
}

bool RemshOperate::CheckingCDT(TriangleArgs& p_TriArgs)
{
    
    for (auto& edge : p_TriArgs.m_CheckingEdges) {

        Mesh::EdgeHandle eh = m_OriginalMesh.edge_handle(edge);
        Mesh::HalfedgeHandle h1 = m_OriginalMesh.halfedge_handle(eh, 0);
        Mesh::HalfedgeHandle h2 = m_OriginalMesh.halfedge_handle(eh, 1);

        if (m_OriginalMesh.is_boundary(h1) || m_OriginalMesh.is_boundary(h2)) continue;

        Mesh::Point p1 = m_OriginalMesh.point(m_OriginalMesh.from_vertex_handle(h1));
        Mesh::Point p2 = m_OriginalMesh.point(m_OriginalMesh.to_vertex_handle(h1));

        Mesh::HalfedgeHandle nxtHeh1 = m_OriginalMesh.next_halfedge_handle(h1);
        Mesh::HalfedgeHandle nxtHeh2 = m_OriginalMesh.next_halfedge_handle(h2);

        Mesh::Point p3 = m_OriginalMesh.point(m_OriginalMesh.to_vertex_handle(nxtHeh1));
        Mesh::Point p4 = m_OriginalMesh.point(m_OriginalMesh.to_vertex_handle(nxtHeh2));

        OpenMesh::Vec3d direc1(p2 - p3);
        OpenMesh::Vec3d direc2(p1 - p3);

        OpenMesh::Vec3d direc3(p2 - p4);
        OpenMesh::Vec3d direc4(p1 - p4);

        double theta1 = acos((direc1 | direc2) / (direc1.length() * direc2.length()));
        double theta2 = acos((direc3 | direc4) / (direc1.length() * direc2.length()));
        if ( (theta1 + theta2) > (PI + m_Tolerance.m_Linear)) {

            return false;
                       
        }

    }   


    return true;
}

RemeshPathEdgeRecord RemshOperate::run()
{
    Initialize();

    //while (!m_ProcessingTriangles.empty())
    //{
    //    auto& triRecord =  *m_ProcessingTriangles.begin();

    //    auto id = triRecord.first;
    //   // id = triRecord.first;
    //    GenerateRemesh(triRecord.first, triRecord.second);

    //    m_ProcessingTriangles.erase(triRecord.first);

    //}


    
    while (!m_WaitingProcessingTriangles.empty())
    {
        auto& triRecord =  *m_WaitingProcessingTriangles.begin();

        auto id = triRecord.first;
        // id = triRecord.first;
        GenerateRemesh(triRecord.first, triRecord.second);

        m_WaitingProcessingTriangles.erase(triRecord.first);

    }

    //维护m_ReturnNewVertex;
    for (auto& itr : m_NewVertexs) {
        m_ReturnNewVertex[itr.second] = itr.first->m_Point3d;       
    }


    m_OriginalMesh.update_face_normals();

    return m_PathEdgeRecord;
}


void RemshOperate::TestTriangle(TriangleArgs& p_Args)
{
    //int i = 0;
 /*   cout << endl;
    cout << "Edge: " << p_Args.m_CheckingEdges.size() << endl;
    cout << "Triangle: " << p_Args.m_NewTriangles.size() << endl;
    cout << endl;*/

    for (auto& edge : p_Args.m_CheckingEdges) {
        Mesh::EdgeHandle eh = m_OriginalMesh.edge_handle(edge);
        assert(m_OriginalMesh.is_valid_handle(eh));      
    }



    list<Mesh::Face*> invalidFace;
    list<Mesh::Face*> validFace;
    for (auto& tri : p_Args.m_NewTriangles) {

        Mesh::FaceHandle fh = m_OriginalMesh.face_handle(tri);
        if (!m_OriginalMesh.is_valid_handle(fh)) {
            assert(false);
            //invalidFace.push_back(tri);

        }
        else {
            //validFace.push_back(tri);       
        }
        
    }

 /*   int si = invalidFace.size();
    p_Args.m_NewTriangles.erase(invalidFace.begin(), invalidFace.end());

    if (si > 0 ) {

        cout << "******************" << endl;
        for (auto& tri : validFace) {

            assert(validFace.size() > 0);

            Mesh::FaceHandle fh = m_OriginalMesh.handle(*tri);

            {
                Mesh::FaceVertexCCWIter fvccwIter = m_OriginalMesh.fv_ccwbegin(fh);

                cout << m_OriginalMesh.point(*(fvccwIter++)) << endl;
                cout << m_OriginalMesh.point(*(fvccwIter++)) << endl;
                cout << m_OriginalMesh.point(*(fvccwIter++)) << endl;

            }

        }
        cout << "******************" << endl;
            
    
    }
    */
    
}

void RemshOperate::TestEdge(vector<MeshHalfEdgeHandle*>& p_Args)
{
    //int i = 0;
    //for (auto& edge : p_Args) {
    //    real num1 = edge->VertexAt(0).x; num1 = edge->VertexAt(0).y; num1 = edge->VertexAt(0).z;
    //    num1 = edge->VertexAt(1).x; num1 = edge->VertexAt(1).y; num1 = edge->VertexAt(1).z;
    //    i++;
    //}

}
