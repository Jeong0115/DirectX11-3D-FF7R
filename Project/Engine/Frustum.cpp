#include "pch.h"
#include "Frustum.h"

#include "Camera.h"

namespace ff7r
{
    Frustum::Frustum(Camera* _Cam)
        : m_pCam(_Cam)
        , m_arrProj{}
        , m_arrFace{}
    {
        //    4 -- 5 
        //   /|   /|  <-- Far
        //  / |  / |
        // 0 -- 1 -6
        // |/   | /   <-- Near
        // 3 -- 2
        m_arrProj[0] = vec3(-1.f, 1.f, 0.f);
        m_arrProj[1] = vec3(1.f, 1.f, 0.f);
        m_arrProj[2] = vec3(1.f, -1.f, 0.f);
        m_arrProj[3] = vec3(-1.f, -1.f, 0.f);

        m_arrProj[4] = vec3(-1.f, 1.f, 1.f);
        m_arrProj[5] = vec3(1.f, 1.f, 1.f);
        m_arrProj[6] = vec3(1.f, -1.f, 1.f);
        m_arrProj[7] = vec3(-1.f, -1.f, 1.f);
    }

    Frustum::~Frustum()
    {
    }

    void Frustum::FinalTick()
    {
        // 투영공간에서서의 NDC 박스 좌표가 역으로 월드로 가면 
        // 월드상의 카메라 시야범위 8개의 모서리 위치일 것이다.
        const Matrix& matProjInv = m_pCam->GetProjInvMat();
        const Matrix& matViewInv = m_pCam->GetViewInvMat();
        Matrix matPVInv = matProjInv * matViewInv;

        vec3 arrWorld[8] = {};
        for (int i = 0; i < 8; ++i)
        {
            arrWorld[i] = XMVector3TransformCoord(m_arrProj[i], matPVInv);
        }

        // 카메라 시야범위를 구성하는 6개의 평면을 구한다.
         //    4 -- 5 
        //   /|   /|  <-- Far
        //  / |  / |
        // 0 -- 1 -6
        // |/   | /   <-- Near
        // 3 -- 2
        m_arrFace[FT_NEAR] = XMPlaneFromPoints(arrWorld[0], arrWorld[1], arrWorld[2]);
        m_arrFace[FT_FAR] = XMPlaneFromPoints(arrWorld[5], arrWorld[4], arrWorld[7]);
        m_arrFace[FT_UP] = XMPlaneFromPoints(arrWorld[4], arrWorld[5], arrWorld[1]);
        m_arrFace[FT_DOWN] = XMPlaneFromPoints(arrWorld[3], arrWorld[2], arrWorld[6]);
        m_arrFace[FT_LEFT] = XMPlaneFromPoints(arrWorld[7], arrWorld[4], arrWorld[0]);
        m_arrFace[FT_RIGHT] = XMPlaneFromPoints(arrWorld[1], arrWorld[5], arrWorld[6]);
    }

    bool Frustum::FrustumCheck(vec3 _vPos)
    {
        for (int i = 0; i < FT_END; ++i)
        {
            vec3 vNormal = m_arrFace[i];
            if (vNormal.Dot(_vPos) + m_arrFace[i].w > 0)
                return false;
        }
        return true;
    }

    bool Frustum::FrustumCheckBound(vec3 _vPos, float _fRadis)
    {
        for (int i = 0; i < FT_END; ++i)
        {
            vec3 vNormal = m_arrFace[i];
            if (vNormal.Dot(_vPos) + m_arrFace[i].w > _fRadis)
                return false;
        }
        return true;
    }
}