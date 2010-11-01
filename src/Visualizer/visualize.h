/*
 * visualize.h
 * class declaration for cvp_vis.
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

#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkExtractVOI.h"
#include "vtkPlaneSource.h"
#include "vtkPNMReader.h"
    
class Visualize {
public:
    Visualize();
    ~Visualize();

	void LoadData(char* fp, int nslices);

    vtkRenderWindow* renWin;
    vtkActor* actor;
    vtkPolyDataMapper* mapper;
    vtkRenderer* ren1;
    vtkRenderWindowInteractor* iren;
 
    vtkPNMReader* readerSegmented;

    char SourcePath[256], DstPath[256], Vessels_FileName[256], buffer[256];
    int N1, N2, m_Slices;
    float m_X, m_Y, m_Z;
};
