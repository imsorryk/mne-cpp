//=============================================================================================================
/**
* @file     brainsurfacetreeitem.cpp
* @author   Lorenz Esch <Lorenz.Esch@tu-ilmenau.de>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     November, 2015
*
* @section  LICENSE
*
* Copyright (C) 2015, Lorenz Esch and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of MNE-CPP authors nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    BrainSurfaceTreeItem class definition.
*
*/

//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include "brainsurfacetreeitem.h"


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================

using namespace DISP3DNEWLIB;


//*************************************************************************************************************
//=============================================================================================================
// DEFINE MEMBER METHODS
//=============================================================================================================

BrainSurfaceTreeItem::BrainSurfaceTreeItem(const Surface &tSurface, const Annotation &tAnnotation, int iType, QString text, Qt3DCore::QEntity *entityParent)
: QStandardItem(text)
, Renderable3DEntity(tSurface.rr(), tSurface.nn(), tSurface.tris(), -tSurface.offset(), entityParent)
, m_iType(iType)
{
    createBrainSurfaceTreeItem(tSurface, tAnnotation);
}


//*************************************************************************************************************

BrainSurfaceTreeItem::~BrainSurfaceTreeItem()
{
}


//*************************************************************************************************************

QVariant BrainSurfaceTreeItem::data(int role) const
{
    switch(role) {
        case BrainTreeModelRoles::GetSurfName:
            return QVariant();

        case BrainTreeModelRoles::GetRenderable3DEntity:
            return QVariant();

    }

    return QStandardItem::data(role);
}


//*************************************************************************************************************

void  BrainSurfaceTreeItem::setData(const QVariant& value, int role)
{
    QStandardItem::setData(value, role);
}


//*************************************************************************************************************

int  BrainSurfaceTreeItem::type() const
{
    return m_iType;
}


//*************************************************************************************************************

bool BrainSurfaceTreeItem::createBrainSurfaceTreeItem(const Surface &tSurface, const Annotation &tAnnotation)
{
    BrainTreeItem *itemSurfFileName = new BrainTreeItem(BrainTreeItemTypes::SurfaceFileName, tSurface.fileName());
    itemSurfFileName->setToolTip("Surface file name");
    this->appendRow(itemSurfFileName);

    BrainTreeItem *itemSurfPath = new BrainTreeItem(BrainTreeItemTypes::SurfaceFilePath, tSurface.filePath());
    itemSurfPath->setToolTip("Surface file path");
    this->appendRow(itemSurfPath);

    BrainTreeItem *itemAnnotFileName = new BrainTreeItem(BrainTreeItemTypes::AnnotFileName, tAnnotation.fileName());
    itemAnnotFileName->setToolTip("Annot file name");
    this->appendRow(itemAnnotFileName);

    BrainTreeItem *itemAnnotPath = new BrainTreeItem(BrainTreeItemTypes::AnnotFilePath, tAnnotation.filePath());
    itemAnnotPath->setToolTip("Annot file path");
    this->appendRow(itemAnnotPath);

    //    QList<QVariant> lDataVariant;

    //    //Actual data of the top level brain item
    //    //FilePath
    //    lDataVariant<<tSurface.filePath();
    //    BrainTreeItem* pBrainTreeItemPath = new BrainTreeItem(lDataVariant, BrainTreeItem::FilePath, pBrainTreeItemTop);
    //    pBrainTreeItemTop->appendChild(pBrainTreeItemPath);
    //    lDataVariant.clear();

    //    //AnnotName
    //    lDataVariant<<(tAnnotation.fileName().isEmpty() ? "unspecified" : tAnnotation.fileName());
    //    BrainTreeItem* pBrainTreeItemAnnotName = new BrainTreeItem(lDataVariant, BrainTreeItem::AnnotName, pBrainTreeItemTop);
    //    pBrainTreeItemTop->appendChild(pBrainTreeItemAnnotName);
    //    lDataVariant.clear();

    //    //SurfType
    //    lDataVariant<<tSurface.surf();
    //    BrainTreeItem* pBrainTreeItemSurfType = new BrainTreeItem(lDataVariant, BrainTreeItem::SurfType, pBrainTreeItemTop);
    //    pBrainTreeItemTop->appendChild(pBrainTreeItemSurfType);
    //    lDataVariant.clear();

    //    //Hemi
    //    lDataVariant<<tSurface.hemi();
    //    BrainTreeItem* pBrainTreeItemHemi = new BrainTreeItem(lDataVariant, BrainTreeItem::Hemi, pBrainTreeItemTop);
    //    pBrainTreeItemTop->appendChild(pBrainTreeItemHemi);
    //    lDataVariant.clear();

    //    //ColorSulci
    //    lDataVariant<<QColor(50,50,50);
    //    BrainTreeItem* pBrainTreeItemColorSulci = new BrainTreeItem(lDataVariant, BrainTreeItem::ColorSulci, pBrainTreeItemTop);
    //    pBrainTreeItemTop->appendChild(pBrainTreeItemColorSulci);
    //    lDataVariant.clear();

    //    //ColorGyri
    //    lDataVariant<<QColor(125,125,125);
    //    BrainTreeItem* pBrainTreeItemColorGyri = new BrainTreeItem(lDataVariant, BrainTreeItem::ColorGyri, pBrainTreeItemTop);
    //    pBrainTreeItemTop->appendChild(pBrainTreeItemColorGyri);
    //    lDataVariant.clear();

    //    //Renderable3DEntity
    //    Renderable3DEntity* pRenderable3DEntity = new Renderable3DEntity(tSurface.rr(), tSurface.nn(), tSurface.tris(), -tSurface.offset(), p3DEntityParent);
    //    QVariant var;
    //    var.setValue(pRenderable3DEntity);
    //    lDataVariant<<var;
    //    BrainTreeItem* pBrainTreeItem3DEntity = new BrainTreeItem(lDataVariant, BrainTreeItem::Renderable3DEntity, pBrainTreeItemTop);
    //    pBrainTreeItemTop->appendChild(pBrainTreeItem3DEntity);
    //    lDataVariant.clear();

    return true;
}
