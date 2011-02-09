#ifndef WXJAD_DROPTARGET_H
#define WXJAD_DROPTARGET_H

#include <wx/dnd.h>

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

class wxJADDropTarget : public wxFileDropTarget
{
public:

    wxJADDropTarget();
    virtual ~wxJADDropTarget() {}

    virtual bool OnDropFiles( wxCoord x, wxCoord y, const wxArrayString& filenames );

};

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

#endif
