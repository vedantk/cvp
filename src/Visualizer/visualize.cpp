/*
 * visualize.cpp
 * class implementation for cvp_vis.
 *
 * Copyright (c) 2009, M. Sabry Hassouna
 * Modified by Vedant Kumar.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * > Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * > Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * > The names of the authors may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <math.h>
#include "visualize.h"
#include "vtkPlaneSource.h"
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkCamera.h"
#include "vtkCellArray.h"
#include "vtkDelaunay3D.h"
#include "vtkDelaunay2D.h"
#include "vtkShrinkFilter.h"
#include "vtkDataSetMapper.h"
#include "vtkImageToStructuredPoints.h"
#include "vtkPNMReader.h"
#include "vtkMarchingCubes.h"
#include "vtkGlyph3D.h"
#include "vtkSphereSource.h"
#include "vtkProperty.h"
#include "vtkPolyDataNormals.h"
#include "vtkDecimate.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkCleanPolyData.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkWindowedSincPolyDataFilter.h"

Visualize::Visualize() {	
    mapper 		= vtkPolyDataMapper::New();
    ren1 		= vtkRenderer::New();    
    actor	 	= vtkActor::New();       
    renWin 		= vtkRenderWindow::New();
    iren 		= vtkRenderWindowInteractor::New(); 
	renWin->AddRenderer(ren1);
	iren->SetRenderWindow(renWin);   
}

void Visualize::LoadData(char* fileprefix, int num_of_slices) {
	N1 = 512;
	N2 = 512;
	m_Slices = num_of_slices;
       
	m_X = 1.0;
	m_Y = 1.0;
	m_Z = 2.6;

    readerSegmented = vtkPNMReader::New();  
    readerSegmented->SetDataExtent(0, N1-1, 0, N2-1, 1, m_Slices);
    readerSegmented->SetDataSpacing(m_X, m_Y, m_Z);
    readerSegmented->SetFilePrefix (fileprefix);
    readerSegmented->SetFilePattern("%s%03d.pgm");     
    
    vtkMarchingCubes *isoSurface = vtkMarchingCubes::New();
    isoSurface->SetInput(readerSegmented->GetOutput());
    isoSurface->GenerateValues(1, 125, 255);   

    vtkCleanPolyData *cleaner = vtkCleanPolyData::New();
    cleaner->SetInput(isoSurface->GetOutput());
    
    vtkDecimate *deci = vtkDecimate::New();
    deci->SetInput(cleaner->GetOutput());
    deci->SetTargetReduction(0.5);
    deci->SetAspectRatio(20);
    deci->SetInitialError(0.0005);
    deci->SetErrorIncrement(0.001);
    deci->SetMaximumIterations(30);
    deci->SetInitialFeatureAngle(30);
    
	vtkWindowedSincPolyDataFilter *smooth = vtkWindowedSincPolyDataFilter::New();
    smooth->SetInput(isoSurface->GetOutput());
    smooth->SetNumberOfIterations(60);
    smooth->BoundarySmoothingOn();
    smooth->SetFeatureAngle(120);
    smooth->SetEdgeAngle(90);
    smooth->SetPassBand(0.09);
    
    vtkPolyDataNormals *normals = vtkPolyDataNormals::New();
    normals->SetInput(smooth->GetOutput());
    normals->SetFeatureAngle(60.0);
    normals->SetFlipNormals(1);

    vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
    mapper->SetInput(normals->GetOutput());
    mapper->ScalarVisibilityOff(); 
    
    vtkActor *VesselActor = vtkActor::New();
    VesselActor->SetMapper(mapper);        
    VesselActor->GetProperty()->SetColor(242.0/255.0,184.0/255.0,77.0/255.0);
    VesselActor->GetProperty()->SetOpacity(0.3);
    ren1->AddActor(VesselActor);

 	ren1->GetActiveCamera()->Elevation(-90);
    ren1->GetActiveCamera()->Azimuth(-180);
	ren1->GetActiveCamera()->Roll(0);

    ren1->SetBackground(1.0,1.0,1.0);
    	
  	renWin->SetSize(600,800);
	renWin->Render();
	iren->Start();

	/*
	delete VesselActor;
	delete normals;
	delete smooth;
	delete deci;
	delete cleaner;
	delete isoSurface;
	*/
}

Visualize::~Visualize() {
	/*
    delete renWin;
    delete actor;
    delete mapper;
    delete ren1;
    delete iren;
    delete readerSegmented;
	*/
}
