BOOL CMyBitmap::GameCameraCreate()
{
	if(s_pCamera)
		CMyBitmap::GameCameraDestroy();
	s_pCamera = new C3Camera;
	if(!s_pCamera)
		return false;
	Camera_Clear ( s_pCamera );
	s_pCamera->lpFrom = new D3DXVECTOR3[1];
	s_pCamera->lpFrom[0].x = _SCR_WIDTH/2;
	s_pCamera->lpFrom[0].y = -1000;
	s_pCamera->lpFrom[0].z = _SCR_HEIGHT/2;
	
	s_pCamera->lpTo = new D3DXVECTOR3[1];
	s_pCamera->lpTo[0].x = _SCR_WIDTH/2;
	s_pCamera->lpTo[0].y = 0;
	s_pCamera->lpTo[0].z = _SCR_HEIGHT/2;
	
	s_pCamera->fNear = 1.0f;
	s_pCamera->fFar = 10000.0f;
	return true;
}
BOOL Camera_BuildView ( C3Camera *lpCamera, BOOL bSet )
{
	D3DXVECTOR3	up ( 0.0f, 0.0f, -1.0f );
	if ( FAILED ( D3DXMatrixLookAtLH ( &g_ViewMatrix,
		&lpCamera->lpFrom[lpCamera->nFrame],
		&lpCamera->lpTo[lpCamera->nFrame],
		&up ) ) )
		return false;
	
	if ( bSet )
	{
		if ( FAILED ( g_D3DDevice->SetTransform ( D3DTS_VIEW, 
			&g_ViewMatrix ) ) )
			return false;
	}
	return true;
}
BOOL Camera_BuildOrtho ( C3Camera *lpCamera,
						float fWidth,
						float fHeight,
						BOOL bSet )
{
	if ( FAILED ( D3DXMatrixOrthoLH ( &g_ProjectMatrix,
		fWidth,
		fHeight,
		lpCamera->fNear,
		lpCamera->fFar ) ) )
		return false;
	
	if ( bSet )
	{
		if ( FAILED ( g_D3DDevice->SetTransform ( D3DTS_PROJECTION,
			&g_ProjectMatrix ) ) )
			return false;
	}
	return true;
}