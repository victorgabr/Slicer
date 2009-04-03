#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include "vtkCommand.h"

#include "vtkSlicerApplication.h"
#include "vtkSlicerApplicationGUI.h"
#include "vtkSlicerGUILayout.h"
#include "vtkSlicerModuleGUI.h"
#include "vtkSlicerToolbarGUI.h"
#include "vtkSlicerWindow.h"
#include "vtkSlicerModuleChooseGUI.h"
#include "vtkSlicerModuleNavigator.h"
#include "vtkSlicerTheme.h"
#include "vtkSlicerColor.h"

#include "vtkMRMLSelectionNode.h"
#include "vtkMRMLViewNode.h"
#include "vtkMRMLLayoutNode.h"

#include "vtkKWTkUtilities.h"
#include "vtkKWWidget.h"
#include "vtkKWToolbarSet.h"
#include "vtkKWMessageDialog.h"
#include "vtkKWFrameWithLabel.h"
#include "vtkKWRadioButton.h"
#include "vtkKWSeparator.h"
#include "vtkKWMenu.h"
#include "vtkKWEntry.h"
#include "vtkKWLabel.h"
#include "vtkKWMessageDialog.h"

//#define LIGHTBOXGUI_DEBUG

//---------------------------------------------------------------------------
vtkStandardNewMacro (vtkSlicerToolbarGUI );
vtkCxxRevisionMacro ( vtkSlicerToolbarGUI, "$Revision: 1.0 $");


//---------------------------------------------------------------------------
vtkSlicerToolbarGUI::vtkSlicerToolbarGUI ( )
{
  vtkKWToolbar::SetGlobalToolbarAspectToUnChanged ( );
  vtkKWToolbar::SetGlobalWidgetsAspectToUnChanged ( );

  this->SlicerToolbarIcons = vtkSlicerToolbarIcons::New ( );

  this->ModulesToolbar = vtkKWToolbar::New ( );
  this->LoadSaveToolbar = vtkKWToolbar::New ( );
  this->ViewToolbar = vtkKWToolbar::New ( );
  this->InteractionModeToolbar = vtkKWToolbar::New ( );
  this->UndoRedoToolbar = vtkKWToolbar::New ( );
  this->UtilitiesToolbar = vtkKWToolbar::New();

  this->ScreenShotIconButton = vtkKWPushButton::New();
  this->HomeIconButton = vtkKWPushButton::New ( );
  this->DataIconButton = vtkKWPushButton::New ( );
  this->VolumeIconButton = vtkKWPushButton::New ( );
  this->ModelIconButton = vtkKWPushButton::New ( );
  this->EditorIconButton = vtkKWPushButton::New ( );
  this->EditorToolboxIconButton = vtkKWPushButton::New ( );
  this->ColorIconButton = vtkKWPushButton::New ( );
  this->FiducialsIconButton = vtkKWPushButton::New ( );
//  this->MeasurementsIconButton = vtkKWPushButton::New ( );
  this->TransformIconButton = vtkKWPushButton::New ( );
  this->SaveSceneIconButton = vtkKWPushButton::New ( );
  this->LoadSceneIconButton = vtkKWMenuButton::New ( );
  this->ChooseLayoutIconMenuButton = vtkKWMenuButton::New();
  this->MousePickButton = vtkKWRadioButton::New();
  this->MousePlaceButton = vtkKWRadioButton::New();
  this->MouseTransformViewButton = vtkKWRadioButton::New();

  this->UndoIconButton = vtkKWPushButton::New ( );
  this->RedoIconButton = vtkKWPushButton::New ( );
  this->ModuleChooseGUI = vtkSlicerModuleChooseGUI::New ( );
  
  this->ApplicationGUI = NULL;
  this->InteractionNodeID = NULL;
  this->InteractionNode = NULL;

  this->ProcessingMRMLEvent = 0;

  this->CompareViewBoxTopLevel = NULL; //vtkKWTopLevel::New( );
  this->CompareViewBoxApplyButton = NULL; //vtkKWPushButton::New( );
  this->CompareViewLightboxRowEntry = NULL;
  this->CompareViewLightboxColumnEntry = NULL;

  //--- Screen snapshot configure window
  this->ScreenShotFormatMenuButton = NULL;
  this->ScreenShotOptionsWindow = NULL;
  this->ScreenShotNameEntry = NULL;
  this->ScreenShotNumberEntry = NULL;
  this->ScreenShotOverwriteButton = NULL;
  this->ScreenShotCaptureButton = NULL;
  this->ScreenShotCloseButton = NULL;
  this->ScreenShotDialogButton = NULL;
  this->ScreenShotMagnificationEntry  = NULL;
  this->ScreenShotOverwrite = 0;
  
  this->ScreenShotDirectory = NULL;
  this->ScreenShotName = NULL;
  this->ScreenShotNumber = 0;
  this->ScreenShotMagnification = 1;
  this->ScreenShotFormat = ".png";

}


//---------------------------------------------------------------------------
vtkSlicerToolbarGUI::~vtkSlicerToolbarGUI ( )
{

  // Remove widgets from Toolbars
  if ( this->ModulesToolbar )
    {
    this->ModulesToolbar->RemoveAllWidgets( );
    }
  if ( this->LoadSaveToolbar )
    {
    this->LoadSaveToolbar->RemoveAllWidgets ( );
    }
  if ( this->ViewToolbar )
    {
    this->ViewToolbar->RemoveAllWidgets ( );
    }
  if ( this->UndoRedoToolbar )
    {
    this->UndoRedoToolbar->RemoveAllWidgets ( );
    }
  if ( this->InteractionModeToolbar )
    {
    this->InteractionModeToolbar->RemoveAllWidgets ( );
    }
  if ( this->UtilitiesToolbar )
    {
    this->UtilitiesToolbar->RemoveAllWidgets();
    }

  if ( this->CompareViewLightboxRowEntry)
    {
    this->CompareViewLightboxRowEntry->SetParent ( NULL );
    this->CompareViewLightboxRowEntry->Delete ( );
    this->CompareViewLightboxRowEntry = NULL;    
    }
  if (this->CompareViewLightboxColumnEntry)
    {
    this->CompareViewLightboxColumnEntry->SetParent ( NULL );
    this->CompareViewLightboxColumnEntry->Delete();
    this->CompareViewLightboxColumnEntry = NULL;    
    }

  if ( this->CompareViewBoxRowEntry )
    {
    this->CompareViewBoxRowEntry->SetParent ( NULL );
    this->CompareViewBoxRowEntry->Delete();
    this->CompareViewBoxRowEntry = NULL;
    }

//  if ( this->CompareViewBoxColumnEntry )
//    {
//    this->CompareViewBoxColumnEntry->SetParent ( NULL );
//    this->CompareViewBoxColumnEntry->Delete();
//    this->CompareViewBoxColumnEntry = NULL;
//    }

  // Delete module choose gui
  if ( this->ModuleChooseGUI )
    {
    this->ModuleChooseGUI->Delete ( );
    this->ModuleChooseGUI = NULL;
    }
    
  // Delete the widgets
  if ( this->ScreenShotMagnificationEntry )
    {
    this->ScreenShotMagnificationEntry->SetParent ( NULL );
    this->ScreenShotMagnificationEntry->Delete();
    this->ScreenShotMagnificationEntry = NULL;    
    }
  if ( this->ScreenShotDialogButton )
    {
    this->ScreenShotDialogButton->SetParent ( NULL );
    this->ScreenShotDialogButton->Delete();
    this->ScreenShotDialogButton = NULL;
    }
  if ( this->ScreenShotIconButton )
    {
    this->ScreenShotIconButton->SetParent ( NULL );
    this->ScreenShotIconButton->Delete();
    this->ScreenShotIconButton = NULL;    
    }
  if ( this->HomeIconButton )
    {
    this->HomeIconButton->SetParent ( NULL );
    this->HomeIconButton->Delete ( );
    this->HomeIconButton = NULL;
    }
  if ( this->DataIconButton )
    {
    this->DataIconButton->SetParent ( NULL );
    this->DataIconButton->Delete ( );
    this->DataIconButton = NULL;
    }
  if ( this->VolumeIconButton )
    {
    this->VolumeIconButton->SetParent ( NULL );
    this->VolumeIconButton->Delete ( );
    this->VolumeIconButton = NULL;
    }
  if ( this->ModelIconButton )
    {
    this->ModelIconButton->SetParent ( NULL );
    this->ModelIconButton->Delete ( );
    this->ModelIconButton = NULL;
    }
  if ( this->EditorIconButton )
    {
    this->EditorIconButton->SetParent ( NULL );
    this->EditorIconButton->Delete ( );
    this->EditorIconButton = NULL;
    }

  if ( this->EditorToolboxIconButton )
    {
    this->EditorToolboxIconButton->SetParent ( NULL );
    this->EditorToolboxIconButton->Delete ( );
    this->EditorToolboxIconButton = NULL;
    }
  if ( this->TransformIconButton )
    {
    this->TransformIconButton->SetParent ( NULL );
    this->TransformIconButton->Delete ( );
    this->TransformIconButton = NULL;
    }
  if ( this->ColorIconButton )
    {
    this->ColorIconButton->SetParent ( NULL );
    this->ColorIconButton->Delete ( );
    this->ColorIconButton = NULL;
    }
  if ( this->FiducialsIconButton )
    {
    this->FiducialsIconButton->SetParent ( NULL );
    this->FiducialsIconButton->Delete ( );
    this->FiducialsIconButton = NULL;
    }
/*
  if ( this->MeasurementsIconButton )
    {
    this->MeasurementsIconButton->SetParent ( NULL );
    this->MeasurementsIconButton->Delete ( );
    this->MeasurementsIconButton = NULL;
    }
*/
  if ( this->SaveSceneIconButton )
    {
    this->SaveSceneIconButton->SetParent ( NULL );
    this->SaveSceneIconButton->Delete ( );
    this->SaveSceneIconButton = NULL;
    }
  if ( this->LoadSceneIconButton )
    {
    this->LoadSceneIconButton->SetParent ( NULL );
    this->LoadSceneIconButton->Delete ( );
    this->LoadSceneIconButton = NULL;
    }
  if ( this->ChooseLayoutIconMenuButton )
    {
    this->ChooseLayoutIconMenuButton->SetParent ( NULL );
    this->ChooseLayoutIconMenuButton->Delete();
    this->ChooseLayoutIconMenuButton = NULL;    
    }
  if ( this->UndoIconButton )
    {
    this->UndoIconButton->SetParent ( NULL );
    this->UndoIconButton->Delete ( );
    this->UndoIconButton = NULL;
    }
  if ( this->RedoIconButton )
    {
    this->RedoIconButton->SetParent ( NULL );
    this->RedoIconButton->Delete ( );
    this->RedoIconButton = NULL;
    }

  // Remove the toolbars from the window's toolbar set
    vtkSlicerApplicationGUI *p = this->GetApplicationGUI ( );
    if ( p ) 
      {
      vtkSlicerWindow *win = this->ApplicationGUI->GetMainSlicerWindow();
      if ( win)
        {
        vtkKWToolbarSet *tbs = win->GetMainToolbarSet ( );
        if (tbs) 
          {
          tbs->RemoveAllToolbars ( );
          }
        }
      }
   
    // Delete the toolbars
    if ( this->ModulesToolbar )
      {
      this->ModulesToolbar->SetParent ( NULL );
      this->ModulesToolbar->Delete ( );
      this->ModulesToolbar = NULL;
      }
    if ( this->LoadSaveToolbar )
      {
      this->LoadSaveToolbar->SetParent ( NULL );
      this->LoadSaveToolbar->Delete ( );
      this->LoadSaveToolbar = NULL;
      }
    if ( this->UndoRedoToolbar )
      {
      this->UndoRedoToolbar->SetParent ( NULL );
      this->UndoRedoToolbar->Delete ( );
      this->UndoRedoToolbar = NULL;
      }
    if ( this->UtilitiesToolbar )
      {
      this->UtilitiesToolbar->SetParent ( NULL );
      this->UtilitiesToolbar->Delete();
      this->UtilitiesToolbar = NULL;
      }
    if ( this->ViewToolbar )
      {
      this->ViewToolbar->SetParent ( NULL );
      this->ViewToolbar->Delete ( );
      this->ViewToolbar = NULL;
      }
    if ( this->InteractionModeToolbar )
      {
      this->InteractionModeToolbar->SetParent ( NULL );
      this->InteractionModeToolbar->Delete ( );
      this->InteractionModeToolbar = NULL;
      }
    if ( this->MousePickButton )
      {
      this->MousePickButton->SetParent ( NULL );
      this->MousePickButton->Delete();
      this->MousePickButton = NULL;
      }
    if ( this->MousePlaceButton )
      {
      this->MousePlaceButton->SetParent ( NULL );
      this->MousePlaceButton->Delete();
      this->MousePlaceButton = NULL;
      }
    if ( this->MouseTransformViewButton )
      {
      this->MouseTransformViewButton->SetParent ( NULL );
      this->MouseTransformViewButton->Delete();
      this->MouseTransformViewButton = NULL;
      }
  // Delete Toolbar Icons
  if ( this->SlicerToolbarIcons )
    {
    this->SlicerToolbarIcons->Delete ( );
    this->SlicerToolbarIcons = NULL;
    }

  if ( this->CompareViewBoxApplyButton )
    {
    this->CompareViewBoxApplyButton->SetParent ( NULL );
    this->CompareViewBoxApplyButton->Delete ( );
    this->CompareViewBoxApplyButton = NULL;
    }
  if ( this->CompareViewBoxTopLevel )
    {
    this->CompareViewBoxTopLevel->SetParent(NULL);
    this->CompareViewBoxTopLevel->Delete  ( );
    this->CompareViewBoxTopLevel = NULL;
    }

  //--- Screen snapshot configure window
  this->DestroyScreenShotOptionsWindow();
  
  this->SetScreenShotDirectory ( NULL );
  this->SetScreenShotName ( NULL );
  this->SetApplicationGUI ( NULL );
  this->SetInteractionNodeID ( NULL );
  vtkSetMRMLNodeMacro( this->InteractionNode, NULL);

}


//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::PrintSelf ( ostream& os, vtkIndent indent )
{
    this->vtkObject::PrintSelf ( os, indent );

    os << indent << "SlicerToolbarGUI: " << this->GetClassName ( ) << "\n";
    //os << indent << "Logic: " << this->GetLogic ( ) << "\n";
    // print widgets?
}


//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::ReconfigureGUIFonts ( )
{
  vtkSlicerApplicationGUI *p = this->GetApplicationGUI ( );  
  // populate the application's 3DView control GUI panel
  if ( p != NULL )
    {
    if ( p->GetApplication() != NULL )
      {
      vtkSlicerApplication *app = vtkSlicerApplication::SafeDownCast( p->GetApplication() );
      if ( this->GetModuleChooseGUI() != NULL )
        {
        this->GetModuleChooseGUI()->GetModulesMenuButton()->SetFont ( app->GetSlicerTheme()->GetApplicationFont1() );
        this->GetModuleChooseGUI()->GetModulesLabel()->SetFont ( app->GetSlicerTheme()->GetApplicationFont1() );
        this->GetModuleChooseGUI()->GetModulesSearchEntry()->SetFont ( app->GetSlicerTheme()->GetApplicationFont1() );
        }
      }
    }
}

//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::RemoveMRMLObservers()
{
}

//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::AddMRMLObservers()
{
}


//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::DestroyScreenShotOptionsWindow ( )
{
  if ( !this->GetScreenShotOptionsWindow() )
    {
    return;
    }
  if ( ! (this->GetScreenShotOptionsWindow()->IsCreated()) )
    {
    vtkErrorMacro ( "DestroyScreenShotOptionsWindow: ScreenShotOptionsWindow is not created." );
    return;
    }
  vtkSlicerApplication *app = vtkSlicerApplication::SafeDownCast ( this->GetApplication() );
  if ( app )
    {
    app->Script ( "grab release %s", this->ScreenShotOptionsWindow->GetWidgetName() );
    }
  this->ScreenShotOptionsWindow->Withdraw();
  
  if ( this->ScreenShotDialogButton )
    {
    this->ScreenShotDialogButton->GetLoadSaveDialog()->RemoveObservers ( vtkKWTopLevel::WithdrawEvent, (vtkCommand *)this->GUICallbackCommand );
    this->ScreenShotDialogButton->SetParent ( NULL );
    this->ScreenShotDialogButton->Delete();
    this->ScreenShotDialogButton = NULL;
    }
  if ( this->ScreenShotNameEntry )
    {
    this->ScreenShotNameEntry->RemoveObservers (vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );
    this->ScreenShotNameEntry->SetParent ( NULL );
    this->ScreenShotNameEntry->Delete();
    this->ScreenShotNameEntry = NULL;
    }
  if ( this->ScreenShotNumberEntry )
    {
    this->ScreenShotNumberEntry->RemoveObservers (vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );
    this->ScreenShotNumberEntry->SetParent ( NULL );
    this->ScreenShotNumberEntry->Delete();
    this->ScreenShotNumberEntry = NULL;    
    }
  if ( this->ScreenShotMagnificationEntry )
    {
    this->ScreenShotMagnificationEntry->RemoveObservers ( vtkKWEntry::EntryValueChangedEvent, (vtkCommand * )this->GUICallbackCommand );
    this->ScreenShotMagnificationEntry->SetParent (NULL );
    this->ScreenShotMagnificationEntry->Delete();
    this->ScreenShotMagnificationEntry = NULL;    
    }
  if ( this->ScreenShotFormatMenuButton )
    {
    this->ScreenShotFormatMenuButton->GetMenu()->RemoveObservers ( vtkKWMenu::MenuItemInvokedEvent, (vtkCommand *) this->GUICallbackCommand );
    this->ScreenShotFormatMenuButton->GetMenu()->DeleteAllItems();
    this->ScreenShotFormatMenuButton->SetParent ( NULL );
    this->ScreenShotFormatMenuButton->Delete();
    this->ScreenShotFormatMenuButton = NULL;
    }
  if ( this->ScreenShotOverwriteButton)
    {
    this->ScreenShotOverwriteButton->RemoveObservers (vtkKWCheckButton::SelectedStateChangedEvent, (vtkCommand *)this->GUICallbackCommand );
    this->ScreenShotOverwriteButton->SetParent ( NULL );
    this->ScreenShotOverwriteButton->Delete();
    this->ScreenShotOverwriteButton = NULL;    
    }
  if ( this->ScreenShotCaptureButton )
    {
    this->ScreenShotCaptureButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
    this->ScreenShotCaptureButton->SetParent ( NULL);
    this->ScreenShotCaptureButton->Delete();
    this->ScreenShotCaptureButton = NULL;    
    }
  if ( this->ScreenShotCloseButton )
    {
    this->ScreenShotCloseButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
    this->ScreenShotCloseButton->SetParent ( NULL );
    this->ScreenShotCloseButton->Delete();
    this->ScreenShotCloseButton = NULL;    
    }
  this->ScreenShotOptionsWindow->Delete();
  this->ScreenShotOptionsWindow = NULL;
}




//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::WithdrawScreenShotOptionsWindow ( )
{
  vtkSlicerApplication *app = vtkSlicerApplication::SafeDownCast ( this->GetApplication() );
  if ( app )
    {
    app->Script ( "grab release %s", this->ScreenShotOptionsWindow->GetWidgetName() );
    }

  this->ScreenShotDialogButton->GetLoadSaveDialog()->RemoveObservers ( vtkKWTopLevel::WithdrawEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotNameEntry->RemoveObservers (vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotNumberEntry->RemoveObservers (vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotMagnificationEntry->RemoveObservers ( vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotFormatMenuButton->GetMenu()->RemoveObservers ( vtkKWMenu::MenuItemInvokedEvent, (vtkCommand *)this->GUICallbackCommand);
  this->ScreenShotOverwriteButton->RemoveObservers (vtkKWCheckButton::SelectedStateChangedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotCaptureButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotCloseButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotOptionsWindow->Withdraw();

}




//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::RaiseScreenShotOptionsWindow ( )
{
  //--- create window if not already created.
  vtkSlicerApplication *app = vtkSlicerApplication::SafeDownCast ( this->GetApplication() );

  if ( this->ScreenShotDirectory == NULL )
    {
    if ( app )
      {
      if ( app->GetTemporaryDirectory() )
        {
        this->SetScreenShotDirectory(app->GetTemporaryDirectory() );
        }
      }
    }
  if ( this->ScreenShotOptionsWindow == NULL )
    {
    int px, py;
    //-- top level container.
    this->ScreenShotOptionsWindow = vtkKWTopLevel::New();
    this->ScreenShotOptionsWindow->SetMasterWindow ( this->GetScreenShotIconButton() );
    this->ScreenShotOptionsWindow->SetApplication ( this->GetApplication() );
    this->ScreenShotOptionsWindow->Create();
    vtkKWTkUtilities::GetWidgetCoordinates( this->GetScreenShotIconButton(), &px, &py );
    this->ScreenShotOptionsWindow->SetPosition ( px + 10, py + 10 );
    this->ScreenShotOptionsWindow->SetBorderWidth ( 1 );
    this->ScreenShotOptionsWindow->SetReliefToFlat();
    this->ScreenShotOptionsWindow->SetTitle ( "Screen Capture Options");
    this->ScreenShotOptionsWindow->SetSize ( 550, 250 );
    this->ScreenShotOptionsWindow->Withdraw();
    this->ScreenShotOptionsWindow->SetDeleteWindowProtocolCommand ( this, "DestroyScreenShotOptionsWindow");

    vtkKWFrame *f1 = vtkKWFrame::New();
    f1->SetParent ( this->ScreenShotOptionsWindow );
    f1->Create();
    f1->SetBorderWidth ( 1 );
    this->Script ( "pack %s -side top -anchor nw -fill x -expand y -padx 0 -pady 1", f1->GetWidgetName() );


    vtkKWLabel *l0 = vtkKWLabel::New();
    l0->SetParent ( f1 );
    l0->Create();
    l0->SetText ( "Directory / folder:" );
    this->ScreenShotDialogButton = vtkKWLoadSaveButton::New();
    this->ScreenShotDialogButton->SetParent ( f1 );
    this->ScreenShotDialogButton->Create();
    if ( this->GetScreenShotDirectory() == NULL )
      {
      this->ScreenShotDialogButton->GetLoadSaveDialog()->RetrieveLastPathFromRegistry ("OpenPath");
      const char *lastpath = this->ScreenShotDialogButton->GetLoadSaveDialog()->GetLastPath();
      if ( lastpath != NULL && !(strcmp(lastpath, "" )) )
        {
        this->ScreenShotDialogButton->SetInitialFileName (lastpath);
        }
      }
    else
      {
      this->ScreenShotDialogButton->GetLoadSaveDialog()->SetLastPath ( this->GetScreenShotDirectory() );
      this->ScreenShotDialogButton->SetInitialFileName ( this->GetScreenShotDirectory() );
      }
    this->ScreenShotDialogButton->TrimPathFromFileNameOff();
    this->ScreenShotDialogButton->SetMaximumFileNameLength (128 );
    this->ScreenShotDialogButton->GetLoadSaveDialog()->ChooseDirectoryOn();

    this->ScreenShotDialogButton->SetBalloonHelpString ( "Select a directory in which screen captures will be saved." );

    vtkKWLabel *l1 = vtkKWLabel::New();
    l1->SetParent ( f1 );
    l1->Create();
    l1->SetText ( "Image name:" );
    this->SetScreenShotName  ("SlicerImage");
    this->ScreenShotNameEntry = vtkKWEntry::New();
    this->ScreenShotNameEntry->SetParent ( f1 );
    this->ScreenShotNameEntry->Create();
    this->ScreenShotNameEntry->SetValue ( this->ScreenShotName );    
    this->ScreenShotNameEntry->SetBalloonHelpString ( "Select a base-name for the image file, or use the default provided." );

    vtkKWLabel *l2 = vtkKWLabel::New();
    l2->SetParent ( f1 );
    l2->Create();
    l2->SetText ( "Image version number:" );
    this->ScreenShotNumberEntry = vtkKWEntry::New();
    this->ScreenShotNumberEntry->SetParent ( f1 );
    this->ScreenShotNumberEntry->Create();
    this->ScreenShotNumberEntry->SetValueAsInt ( this->ScreenShotNumber );
    this->ScreenShotNumberEntry->SetBalloonHelpString ( "Select a number to append to the image file base-name to create a unique filename." );

    vtkKWLabel *l3 = vtkKWLabel::New();
    l3->SetParent ( f1 );
    l3->Create();
    l3->SetText ( "Image scale:" );
    this->ScreenShotMagnificationEntry = vtkKWEntry::New();
    this->ScreenShotMagnificationEntry->SetParent ( f1 );
    this->ScreenShotMagnificationEntry->Create();
    this->ScreenShotMagnificationEntry->SetValueAsInt ( this->ScreenShotMagnification );
    this->ScreenShotMagnificationEntry->SetBalloonHelpString ( "Select a scale factor for the image file, e.g. a value of \"2\" will save an image twice the size of the current 3D Viewer." );

    vtkKWLabel *l4 = vtkKWLabel::New();
    l4->SetParent (f1);
    l4->Create();
    l4->SetText ( "Choose Format:" );
    this->ScreenShotFormatMenuButton = vtkKWMenuButton::New();
    this->ScreenShotFormatMenuButton->SetParent ( f1 );
    this->ScreenShotFormatMenuButton->Create();
    this->ScreenShotFormatMenuButton->GetMenu()->AddRadioButton ( ".png" );
    this->ScreenShotFormatMenuButton->GetMenu()->AddRadioButton ( ".jpg" );
    this->ScreenShotFormatMenuButton->GetMenu()->AddRadioButton ( ".tiff" );
    this->ScreenShotFormatMenuButton->GetMenu()->AddRadioButton ( ".eps" );
    this->ScreenShotFormatMenuButton->GetMenu()->AddRadioButton ( ".ps" );
    this->ScreenShotFormatMenuButton->GetMenu()->AddRadioButton ( ".prn" );
    this->ScreenShotFormatMenuButton->GetMenu()->AddRadioButton ( ".pnm" );
    this->ScreenShotFormatMenuButton->GetMenu()->AddRadioButton ( ".ppm" );
    this->ScreenShotFormatMenuButton->GetMenu()->AddSeparator();
    this->ScreenShotFormatMenuButton->GetMenu()->AddCommand ( "close" );
    this->ScreenShotFormat.clear();
    this->ScreenShotFormat = ".png";
    this->ScreenShotFormatMenuButton->SetWidth ( 15 );
    this->ScreenShotFormatMenuButton->SetValue ( this->ScreenShotFormat.c_str() );
    this->ScreenShotFormatMenuButton->SetBalloonHelpString ( "Specify an image file format." );
    
    vtkKWLabel *l5 = vtkKWLabel::New();
    l5->SetParent (f1);
    l5->Create();
    l5->SetText  ( "Overwrite existing any files" );
    this->ScreenShotOverwriteButton = vtkKWCheckButton::New();
    this->ScreenShotOverwriteButton->SetParent ( f1 );
    this->ScreenShotOverwriteButton->Create();
    this->ScreenShotOverwriteButton->SetSelectedState ( this->ScreenShotOverwrite );
    this->ScreenShotOverwriteButton->SetBalloonHelpString ( "Select this option if you wish to overwrite any existing image files with new screen captures." );

    this->Script ( "grid %s -row 0 -column 0 -padx 2 -pady 2 -sticky e", l0->GetWidgetName() );
    this->Script ( "grid %s -row 0 -column 1  -columnspan 2 -padx 2 -pady 2 -sticky ew", this->ScreenShotDialogButton->GetWidgetName() );
    this->Script ( "grid %s -row 1 -column 0 -padx 2 -pady 2 -sticky e", l1->GetWidgetName() );
    this->Script ( "grid %s -row 1 -column 1  -columnspan 2 -padx 2 -pady 2 -sticky ew", this->ScreenShotNameEntry->GetWidgetName() );
    this->Script ( "grid %s -row 2 -column 0 -padx 2 -pady 2 -sticky e", l2->GetWidgetName() );
    this->Script ( "grid %s -row 2 -column 1  -padx 2 -pady 2 -sticky ew", this->ScreenShotNumberEntry->GetWidgetName() );
    this->Script ( "grid %s -row 3 -column 0 -padx 2 -pady 2 -sticky e", l3->GetWidgetName() );
    this->Script ( "grid %s -row 3 -column 1  -padx 2 -pady 2 -sticky ew", this->ScreenShotMagnificationEntry->GetWidgetName() );
    this->Script ( "grid %s -row 4 -column 0 -padx 2 -pady 2 -sticky e", l4->GetWidgetName() );
    this->Script ( "grid %s -row 4 -column 1  -padx 2 -pady 2 -sticky w", this->ScreenShotFormatMenuButton->GetWidgetName() );
    this->Script ( "grid %s -row 5 -column 0 -padx 2 -pady 2 -sticky e", this->ScreenShotOverwriteButton->GetWidgetName() );
    this->Script ( "grid %s -row 5 -column 1  -columnspan 2 -padx 2 -pady 2 -sticky w", l5->GetWidgetName() );
    this->Script ( "grid columnconfigure %s 0 -weight 0", f1->GetWidgetName() );
    this->Script ( "grid columnconfigure %s 1 -weight 0", f1->GetWidgetName() );
    this->Script ( "grid columnconfigure %s 2 -weight 1", f1->GetWidgetName() );

    
    vtkKWFrame *f2 = vtkKWFrame::New();
    f2->SetParent ( this->ScreenShotOptionsWindow );
    f2->Create();
    f2->SetBorderWidth ( 1 );
    this->Script ( "pack %s -side top -anchor nw -fill x -expand y -padx 0 -pady 3", f2->GetWidgetName() );

    this->ScreenShotCaptureButton = vtkKWPushButton::New();
    this->ScreenShotCaptureButton->SetParent ( f2 );
    this->ScreenShotCaptureButton->Create();
    this->ScreenShotCaptureButton->SetText ( "Capture" );
    this->ScreenShotCaptureButton->SetWidth ( 9 );
    
    this->ScreenShotCloseButton = vtkKWPushButton::New();
    this->ScreenShotCloseButton->SetParent ( f2 );
    this->ScreenShotCloseButton->Create();
    this->ScreenShotCloseButton->SetText ( "Close" );
    this->ScreenShotCloseButton->SetCommand ( this, "WithdrawScreenShotOptionsWindow" );
    this->ScreenShotCloseButton->SetWidth ( 9 );

    this->Script ( "pack %s %s -side left -anchor c -fill x -expand n -padx 0 -pady 1",
                   this->ScreenShotCloseButton->GetWidgetName(),
                   this->ScreenShotCaptureButton->GetWidgetName() );
    

    l0->Delete();
    l1->Delete();
    l2->Delete();
    l3->Delete();
    l4->Delete();
    l5->Delete();
    f1->Delete();
    f2->Delete();

    }


  this->ScreenShotDialogButton->GetLoadSaveDialog()->AddObserver ( vtkKWTopLevel::WithdrawEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotNameEntry->AddObserver (vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotNumberEntry->AddObserver (vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotMagnificationEntry->AddObserver ( vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotFormatMenuButton->GetMenu()->AddObserver ( vtkKWMenu::MenuItemInvokedEvent, (vtkCommand *)this->GUICallbackCommand);
  this->ScreenShotOverwriteButton->AddObserver (vtkKWCheckButton::SelectedStateChangedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotCaptureButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotCloseButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );

  // display
  this->ScreenShotOptionsWindow->DeIconify();
  this->ScreenShotOptionsWindow->Raise();
  if ( app )
    {
    app->Script ( "grab %s", this->ScreenShotOptionsWindow->GetWidgetName() );
    app->ProcessIdleTasks();
    }
  this->Script ( "update idletasks");

}



//---------------------------------------------------------------------------
const char *vtkSlicerToolbarGUI::GetScreenShotFormat ( )
{
  if ( ScreenShotFormat.c_str() != NULL && (strcmp(ScreenShotFormat.c_str(), "" ) ) )
    {
    return ( ScreenShotFormat.c_str() );
    }
  else
    {
    return NULL;
    }
}


//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::SetScreenShotFormat ( const char *format )
{
  this->ScreenShotFormat.clear();
  this->ScreenShotFormat = format;
}


//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::RemoveGUIObservers ( )
{
    // Fill in
  this->LoadSceneIconButton->GetMenu()->RemoveObservers ( vtkKWMenu::MenuItemInvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->SaveSceneIconButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->HomeIconButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->DataIconButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->EditorIconButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->EditorToolboxIconButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->VolumeIconButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ModelIconButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->FiducialsIconButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ColorIconButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ChooseLayoutIconMenuButton->GetMenu()->RemoveObservers ( vtkKWMenu::MenuItemInvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->UndoIconButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->RedoIconButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->MousePickButton->RemoveObservers( vtkKWRadioButton::SelectedStateChangedEvent, (vtkCommand *)this->GUICallbackCommand);
  this->MousePlaceButton->RemoveObservers( vtkKWRadioButton::SelectedStateChangedEvent, (vtkCommand *)this->GUICallbackCommand);
  this->MouseTransformViewButton->RemoveObservers( vtkKWRadioButton::SelectedStateChangedEvent, (vtkCommand *)this->GUICallbackCommand);
  this->CompareViewBoxApplyButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotIconButton->RemoveObservers (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );

  this->CompareViewBoxRowEntry->RemoveObservers ( vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->CompareViewLightboxRowEntry->RemoveObservers ( vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->CompareViewLightboxColumnEntry->RemoveObservers  ( vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );

  if ( this->ModuleChooseGUI != NULL )
    {
    this->ModuleChooseGUI->RemoveGUIObservers();
    }
}

//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::AddGUIObservers ( )
{
  // Fill in
  // add observers onto the module icon buttons 
  this->LoadSceneIconButton->GetMenu()->AddObserver ( vtkKWMenu::MenuItemInvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->SaveSceneIconButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->HomeIconButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->DataIconButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->EditorIconButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->EditorToolboxIconButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->VolumeIconButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ModelIconButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->FiducialsIconButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->TransformIconButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ColorIconButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->ScreenShotIconButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
    
  // view configuration icon button observers...
  this->ChooseLayoutIconMenuButton->GetMenu()->AddObserver ( vtkKWMenu::MenuItemInvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->UndoIconButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->RedoIconButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->MousePickButton->AddObserver( vtkKWRadioButton::SelectedStateChangedEvent, (vtkCommand *)this->GUICallbackCommand);
  this->MousePlaceButton->AddObserver( vtkKWRadioButton::SelectedStateChangedEvent, (vtkCommand *)this->GUICallbackCommand);
  this->MouseTransformViewButton->AddObserver( vtkKWRadioButton::SelectedStateChangedEvent, (vtkCommand *)this->GUICallbackCommand);
  this->CompareViewBoxApplyButton->AddObserver (vtkKWPushButton::InvokedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->CompareViewBoxRowEntry->AddObserver ( vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->CompareViewLightboxRowEntry->AddObserver ( vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );
  this->CompareViewLightboxColumnEntry->AddObserver  ( vtkKWEntry::EntryValueChangedEvent, (vtkCommand *)this->GUICallbackCommand );
  
  if ( this->ModuleChooseGUI != NULL )
    {
    this->ModuleChooseGUI->AddGUIObservers();
    }
}


//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::ProcessGUIEvents ( vtkObject *caller,
                                          unsigned long event, void *callData )
{
  int val;
  if ( this->GetApplicationGUI() != NULL )
    {
    // Toolbar's parent is the main vtkSlicerApplicationGUI;
    // Toolbar events will trigger vtkSlicerAppliationGUI methods
    vtkSlicerApplicationGUI *p = vtkSlicerApplicationGUI::SafeDownCast( this->GetApplicationGUI ( ));
    vtkSlicerApplication *app = vtkSlicerApplication::SafeDownCast( p->GetApplication() );
    vtkMRMLInteractionNode *interactionNode = NULL;

    vtkKWEntry *e = vtkKWEntry::SafeDownCast ( caller );
    vtkKWCheckButton *cb = vtkKWCheckButton::SafeDownCast ( caller );
    vtkKWLoadSaveDialog *d = vtkKWLoadSaveDialog::SafeDownCast ( caller );
    vtkKWRadioButton *radiob = vtkKWRadioButton::SafeDownCast ( caller );
    vtkKWPushButton *pushb = vtkKWPushButton::SafeDownCast ( caller );
    vtkKWMenu *menu = vtkKWMenu::SafeDownCast ( caller );

    vtkMRMLLayoutNode *layout;    

    if (p != NULL)
      {
      //interactionNode = vtkMRMLInteractionNode::SafeDownCast (p->GetMRMLScene()->GetNthNodeByClass(0, "vtkMRMLInteractionNode"));
      }
    interactionNode = this->GetApplicationLogic()->GetInteractionNode();

    if (interactionNode == NULL)
      {
      vtkErrorMacro("ProcessGUIEvents: no interaction node in the scene, not updating the interaction mode!");
      }
                                   
    // Process events from top row of buttons
    if ( app != NULL )
      {
  
      // Mouse mode buttons:
      if ( radiob != NULL )
        {
        if ( radiob == this->MousePickButton
             && event == vtkKWRadioButton::SelectedStateChangedEvent )
          {
          val = radiob->GetSelectedState();
          if ( val && interactionNode )
            {
            interactionNode->SetLastInteractionMode ( interactionNode->GetCurrentInteractionMode() );
            interactionNode->SetCurrentInteractionMode ( vtkMRMLInteractionNode::PickManipulate );
            }
          }
        else if ( radiob == this->MouseTransformViewButton
                  && event == vtkKWRadioButton::SelectedStateChangedEvent)
          {
          val = radiob->GetSelectedState();
          if ( val && interactionNode )
            {
            interactionNode->SetLastInteractionMode ( interactionNode->GetCurrentInteractionMode() );
            interactionNode->SetCurrentInteractionMode ( vtkMRMLInteractionNode::ViewTransform );
            }
          }
        else if ( radiob == this->MousePlaceButton
                  && event == vtkKWRadioButton::SelectedStateChangedEvent)
          {
          val = radiob->GetSelectedState();
          if ( val && interactionNode )
            {
            interactionNode->SetLastInteractionMode ( interactionNode->GetCurrentInteractionMode() );
            interactionNode->SetCurrentInteractionMode ( vtkMRMLInteractionNode::Place );
            }
          }
        }

      if ( e != NULL )
        {
        if ( e == this->ScreenShotNameEntry && event == vtkKWEntry::EntryValueChangedEvent )
          {
          // check
          if ( (this->ScreenShotNameEntry->GetValue() != NULL) && ((strcmp (this->ScreenShotNameEntry->GetValue(), "" ))) )
            {
            this->SetScreenShotName ( this->ScreenShotNameEntry->GetValue() );
            }
          else
            {
            // dialog
            if ( this->GetApplicationGUI() && this->GetApplicationGUI()->GetMainSlicerWindow() )
              {
              vtkKWMessageDialog *dialog = vtkKWMessageDialog::New();
              dialog->SetParent (  this->GetApplicationGUI()->GetMainSlicerWindow() );
              dialog->SetStyleToMessage();
              std::string msg = "Please enter a valid name for the screen capture.";
              dialog->SetText(msg.c_str());
              dialog->Create ( );
              dialog->Invoke();
              dialog->Delete();
              }
            }
          }
        else if ( e == this->ScreenShotNumberEntry && event == vtkKWEntry::EntryValueChangedEvent )
          {
          if ( (this->ScreenShotNumberEntry->GetValueAsInt() >= 0 ) )
            {
            this->ScreenShotNumber = this->ScreenShotNumberEntry->GetValueAsInt ( );
            }
          else
            {
            // dialog
            if ( this->GetApplicationGUI() && this->GetApplicationGUI()->GetMainSlicerWindow() )
              {
              vtkKWMessageDialog *dialog = vtkKWMessageDialog::New();
              dialog->SetParent (  this->GetApplicationGUI()->GetMainSlicerWindow() );
              dialog->SetStyleToMessage();
              std::string msg = "Please enter a non-negative integer value.";
              dialog->SetText(msg.c_str());
              dialog->Create ( );
              dialog->Invoke();
              dialog->Delete();
              }
            }
          }
        else if ( e == this->ScreenShotMagnificationEntry  && event == vtkKWEntry::EntryValueChangedEvent )
          {
          if (this->ScreenShotMagnificationEntry->GetValueAsInt() > 0 )
            {
            this->ScreenShotMagnification = this->ScreenShotMagnificationEntry->GetValueAsInt ( );
            }
          else
            {
            // dialog
            if ( this->GetApplicationGUI() && this->GetApplicationGUI()->GetMainSlicerWindow() )
              {
              vtkKWMessageDialog *dialog = vtkKWMessageDialog::New();
              dialog->SetParent (  this->GetApplicationGUI()->GetMainSlicerWindow() );
              dialog->SetStyleToMessage();
              std::string msg = "Please enter a non-zero integer value.";
              dialog->SetText(msg.c_str());
              dialog->Create ( );
              dialog->Invoke();
              dialog->Delete();
              }
            }
          }
        }
        
      if ( cb != NULL )
        {
        if ( cb == this->ScreenShotOverwriteButton && event == vtkKWCheckButton::SelectedStateChangedEvent )
          {
          this->ScreenShotOverwrite = this->ScreenShotOverwriteButton->GetSelectedState();
          }
        }

      if ( d != NULL && event == vtkKWTopLevel::WithdrawEvent )
        {
        if ( this->ScreenShotDialogButton && d == this->ScreenShotDialogButton->GetLoadSaveDialog() )
          {
          std::string dirname;
          if ( this->ScreenShotDialogButton->GetLoadSaveDialog()->GetFileName() != NULL )
            {
            dirname = this->ScreenShotDialogButton->GetLoadSaveDialog()->GetFileName();
            if ( vtksys::SystemTools::FileIsDirectory ( dirname.c_str() ) )
              {
              this->SetScreenShotDirectory ( dirname.c_str() );
              }
            }
          else
            {
            if ( this->ScreenShotDialogButton->GetLoadSaveDialog()->GetLastPath() != NULL )
              {
              dirname = this->ScreenShotDialogButton->GetLoadSaveDialog()->GetLastPath();
              if ( vtksys::SystemTools::FileIsDirectory ( dirname.c_str() ) )
                {
                this->SetScreenShotDirectory ( dirname.c_str() );
                }
              }
            else
              {
              this->SetScreenShotDirectory ( NULL );
              }
            }
          }
        }

      if ( pushb != NULL && event == vtkKWPushButton::InvokedEvent )
        {
        layout = p->GetGUILayoutNode();
        if ( pushb == this->HomeIconButton )
          {
          const char *homename = app->GetHomeModule();
          vtkSlicerModuleGUI *m = app->GetModuleGUIByName(homename);
          if ( m != NULL && this->GetModuleChooseGUI() != NULL )
            {
            this->GetModuleChooseGUI()->SelectModule ( homename );
            }
          else
            {
            vtkErrorMacro ("ERROR:  no slicer module gui found for Home module '" << (homename ? homename : "null") << "'"); 
            }
          }
        else if ( pushb == this->ScreenShotCaptureButton )
          {
          //--- check for bugs
          //--- try first setting user's selection. if nothing there, choose the last path. if nothing there, mark for error.
          std::string dirname;
          if ( this->GetScreenShotDirectory() != NULL )
            {
            dirname = this->GetScreenShotDirectory();
            }
          else
            {
            if ( this->ScreenShotDialogButton->GetLoadSaveDialog()->GetLastPath() != NULL )
              {
              dirname = this->ScreenShotDialogButton->GetLoadSaveDialog()->GetLastPath();
              }
            else
              {
              dirname = "none";
              }
            }
          if ( (!(strcmp(dirname.c_str(), "none")) ) ||
               (this->ScreenShotName == NULL ) ||
               (this->ScreenShotMagnification < 1 ) ||
               (this->ScreenShotNumber < 0 ) ||
               (this->ScreenShotFormat.c_str() == NULL ) )
            {
            // provide a warning that no directory exists.
            vtkKWMessageDialog *dialog = vtkKWMessageDialog::New();
            dialog->SetParent (  this->GetApplicationGUI()->GetMainSlicerWindow() );
            dialog->SetStyleToMessage();
            std::string msg = "Please specify a valid directory,  filename, version number, scale and format. ";
            dialog->SetText(msg.c_str());
            dialog->Create ( );
            dialog->Invoke();
            dialog->Delete();
            }
          else
            {
            //--- assemble filename with path.
            std::vector<std::string> pathComponents;
            vtksys::SystemTools::SplitPath ( dirname.c_str(), pathComponents );
            std::stringstream ss;
            ss << this->ScreenShotName << "_" << this->ScreenShotNumber << this->ScreenShotFormat;
            std::string s = ss.str();
            pathComponents.push_back ( s );
            std::string filename = vtksys::SystemTools::JoinPath ( pathComponents );
            //--- make sure it's a unix-style path.
            std::string upath = vtksys::SystemTools::ConvertToUnixOutputPath ( filename.c_str() );
            int capture = 1;

            //--- see if file already exists
            if ( vtksys::SystemTools::FileExists ( upath.c_str()) ||
                 vtksys::SystemTools::FileExists ( filename.c_str()) )
              {
              //--- if overwrite not selected and file exists, give a warning & option to overwrite.
              if (!(this->ScreenShotOverwrite) )
                {
                if ( this->GetApplicationGUI() && this->GetApplicationGUI()->GetMainSlicerWindow() )
                  {
                  vtkKWMessageDialog *dialog = vtkKWMessageDialog::New();
                  dialog->SetParent (  this->GetApplicationGUI()->GetMainSlicerWindow() );
                  dialog->SetStyleToYesNo();
                  std::string msg = "File already exists. Overwrite?";
                  dialog->SetText(msg.c_str());
                  dialog->Create ( );
                  capture = dialog->Invoke();
                  dialog->Delete();
                  }
                }
              }
            //--- if good for capture, set status text and grab the screenshot.
            if ( capture )
              {
              if ( this->GetApplicationGUI() && this->GetApplicationGUI()->GetMainSlicerWindow() )
                {
                this->GetApplicationGUI()->GetMainSlicerWindow()->SetStatusText ( "Capturing Screenshot...." );
                }
              this->Script ( "SlicerSaveLargeImage %s %d", upath.c_str(), this->GetScreenShotMagnification() );
              this->ScreenShotNumber += 1;
              this->ScreenShotNumberEntry->SetValueAsInt ( this->ScreenShotNumber );
              if ( this->GetApplicationGUI() && this->GetApplicationGUI()->GetMainSlicerWindow() )
                {
                this->GetApplicationGUI()->GetMainSlicerWindow()->SetStatusText ( "" );
                }
              }
            }
          }
        else if (pushb == this->ScreenShotIconButton )
          {
          this->RaiseScreenShotOptionsWindow();
          }
        else if (pushb == this->DataIconButton )
          {
          vtkSlicerModuleGUI *m = app->GetModuleGUIByName("Data");
          if ( m != NULL && this->GetModuleChooseGUI() != NULL )
            {
            this->GetModuleChooseGUI()->SelectModule ( "Data" );
            }
          else
            {
            vtkDebugMacro ("ERROR:  no slicer module gui found for Data\n");
            }
          }
        else if (pushb == this->VolumeIconButton )
          {
          vtkSlicerModuleGUI *m = app->GetModuleGUIByName("Volumes");
          if ( m != NULL && this->GetModuleChooseGUI() != NULL)
            {
            this->GetModuleChooseGUI()->SelectModule ( "Volumes" );
            }
          else
            {
            vtkDebugMacro ("ERROR:  no slicer module gui found for Volumes\n");
            vtkKWMessageDialog *message = vtkKWMessageDialog::New();
            message->SetParent ( this->VolumeIconButton->GetParent() );
            message->SetStyleToMessage();
            std::string msg = "The Volumes module is not loaded, please check View, Application Settings, Module Settings";
            message->SetText(msg.c_str());
            message->Create();
            message->Invoke();
            message->Delete();
            }
          }
        else if (pushb == this->ModelIconButton )
          {
          vtkSlicerModuleGUI *m = app->GetModuleGUIByName("Models");
          if ( m != NULL  && this->GetModuleChooseGUI() != NULL)
            {
            this->GetModuleChooseGUI()->SelectModule ( "Models" );
            }
          else
            {
            vtkDebugMacro ("ERROR:  no slicer module gui found for Models\n");
            }
          }
        else if (pushb == this->FiducialsIconButton )
          {
          vtkSlicerModuleGUI *m = app->GetModuleGUIByName("Fiducials");
          if ( m != NULL  && this->GetModuleChooseGUI() != NULL)
            {
            this->GetModuleChooseGUI()->SelectModule ( "Fiducials" );
            }
          else
            {
            vtkDebugMacro ("ERROR:  no slicer module gui found for Fiducials\n");
            }
          }
        else if (pushb == this->ColorIconButton )
          {
          vtkSlicerModuleGUI *m = app->GetModuleGUIByName("Color");
          if ( m != NULL  && this->GetModuleChooseGUI() != NULL)
            {
            this->GetModuleChooseGUI()->SelectModule ( "Color" );
            }
          else
            {
            vtkDebugMacro ("ERROR:  no slicer module gui found for Color\n");
            }
          }
        else if (pushb == this->TransformIconButton )
          {
          vtkSlicerModuleGUI *m = app->GetModuleGUIByName("Transforms");
          if ( m != NULL  && this->GetModuleChooseGUI() != NULL)
            {
            this->GetModuleChooseGUI()->SelectModule ( "Transforms" );
            }
          else
            {
            vtkDebugMacro ("ERROR:  no slicer module gui found for Transforms\n");
            }
          }
        else if (pushb == this->EditorIconButton )
          {
          vtkSlicerModuleGUI *m = app->GetModuleGUIByName("Editor");
          if ( m != NULL  && this->GetModuleChooseGUI() != NULL)
            {
            this->GetModuleChooseGUI()->SelectModule ( "Editor" );
            }
          else
            {
            vtkDebugMacro ("ERROR:  no slicer module gui found for Editor\n");
            }
          }
        else if (pushb == this->EditorToolboxIconButton )
          {
          //---
          //--- Add code to pop up Editor toolbox here
          //---
          app->Script ("::EditBox::ShowDialog");
          }

        else if (pushb == this->CompareViewBoxApplyButton) 
          {
          this->HideCompareViewCustomLayoutFrame();
          layout->SetNumberOfCompareViewRows ( this->CompareViewBoxRowEntry->GetValueAsInt() );
          layout->SetNumberOfCompareViewLightboxRows ( this->CompareViewLightboxRowEntry->GetValueAsInt () );
          layout->SetNumberOfCompareViewLightboxColumns ( this->CompareViewLightboxColumnEntry->GetValueAsInt() );
          layout->SetViewArrangement (vtkMRMLLayoutNode::SlicerLayoutCompareView );
          }
        else if ( pushb == this->UndoIconButton )
          {
          p->GetMRMLScene()->Undo();
          }
        else if ( pushb == this->RedoIconButton )
          {
          p->GetMRMLScene()->Redo();
          }
        else if ( pushb == this->SaveSceneIconButton )
          {
          p->ProcessSaveSceneAsCommand();
          }
        }


      // TODO: figure out why we can't resume view rock or spin.
      if ( menu != NULL && event == vtkKWMenu::MenuItemInvokedEvent )
        {
        if ( this->ScreenShotFormatMenuButton != NULL && menu == this->ScreenShotFormatMenuButton->GetMenu() )
          {
          this->SetScreenShotFormat ( this->ScreenShotFormatMenuButton->GetValue() );
          }
        else if ( this->ChooseLayoutIconMenuButton != NULL && menu == this->ChooseLayoutIconMenuButton->GetMenu() )
          {
          if ( p->GetGUILayoutNode() == NULL )
            {
            //--- if there's no layout node yet, create it,
            //--- add it to the scene, and make the
            //--- applicationGUI observe it.
            layout = vtkMRMLLayoutNode::New();
            if (layout == NULL)
              {
              vtkErrorMacro ( "ERROR: No layout node exists and cannot create a new one." );
              return;
              }
            this->MRMLScene->AddNode(layout);
            p->SetAndObserveGUILayoutNode ( layout );
            //--- update MRML selection node.
            if ( this->ApplicationLogic != NULL )
              {
              if ( this->ApplicationLogic->GetSelectionNode() != NULL )
                {
                this->ApplicationLogic->GetSelectionNode()->SetActiveLayoutID( layout->GetID() );
                }
              }
            layout->Delete();
            }
          layout = p->GetGUILayoutNode();

          const char *whichLayout = this->ChooseLayoutIconMenuButton->GetValue();
          if ( !strcmp ( whichLayout, "Conventional layout"))
            {
            if ( layout->GetViewArrangement() != vtkMRMLLayoutNode::SlicerLayoutConventionalView )
              {
              p->GetMRMLScene()->SaveStateForUndo ( layout );
              layout->SetViewArrangement (vtkMRMLLayoutNode::SlicerLayoutConventionalView);
              }
            }
          else if (!strcmp( whichLayout, "3D only layout" ))
            {
            if ( layout->GetViewArrangement() != vtkMRMLLayoutNode::SlicerLayoutOneUp3DView )
              {
              p->GetMRMLScene()->SaveStateForUndo ( layout );
              layout->SetViewArrangement (vtkMRMLLayoutNode::SlicerLayoutOneUp3DView);
              }
            }
          else if ( !strcmp ( whichLayout, "Four-up layout"))
            {
            if ( layout->GetViewArrangement() != vtkMRMLLayoutNode::SlicerLayoutFourUpView )
              {
              p->GetMRMLScene()->SaveStateForUndo ( layout );
              layout->SetViewArrangement (vtkMRMLLayoutNode::SlicerLayoutFourUpView);
              }
            }
          else if ( !strcmp (whichLayout, "Tabbed 3D layout") )
            {
            if ( layout->GetViewArrangement() != vtkMRMLLayoutNode::SlicerLayoutTabbed3DView )
              {
              p->GetMRMLScene()->SaveStateForUndo ( layout );
              layout->SetViewArrangement (vtkMRMLLayoutNode::SlicerLayoutTabbed3DView);
              }
            }
          else if (!strcmp ( whichLayout, "Tabbed slice layout"))
            {
            if ( layout->GetViewArrangement() != vtkMRMLLayoutNode::SlicerLayoutTabbedSliceView )
              {
              p->GetMRMLScene()->SaveStateForUndo ( layout );
              layout->SetViewArrangement (vtkMRMLLayoutNode::SlicerLayoutTabbedSliceView );
              }
            }
          else if (!strcmp ( whichLayout, "Compare layout"))
            {
            PopUpCompareViewCustomLayoutFrame();
            }
          else if ( !strcmp (whichLayout, "Red slice only layout") )
            {
            if ( layout->GetViewArrangement() != vtkMRMLLayoutNode::SlicerLayoutOneUpRedSliceView )
              {
              p->GetMRMLScene()->SaveStateForUndo ( layout );
              layout->SetViewArrangement (vtkMRMLLayoutNode::SlicerLayoutOneUpRedSliceView);
              }
            }
          else if ( !strcmp (whichLayout, "Yellow slice only layout") )
            {
            if ( layout->GetViewArrangement() != vtkMRMLLayoutNode::SlicerLayoutOneUpYellowSliceView )
              {
              p->GetMRMLScene()->SaveStateForUndo ( layout );
              layout->SetViewArrangement (vtkMRMLLayoutNode::SlicerLayoutOneUpYellowSliceView);
              }
            } 
          else if ( !strcmp (whichLayout, "Green slice only layout") )
            {
            if ( layout->GetViewArrangement() != vtkMRMLLayoutNode::SlicerLayoutOneUpGreenSliceView )
              {
              p->GetMRMLScene()->SaveStateForUndo ( layout );
              layout->SetViewArrangement (vtkMRMLLayoutNode::SlicerLayoutOneUpGreenSliceView);
              }
            }
          else if ( !strcmp ( whichLayout, "Toggle GUI panel visibility"))
            {
            int v = p->GetMainSlicerWindow()->GetMainPanelVisibility();
            p->GetMainSlicerWindow()->SetMainPanelVisibility (!v );
            layout->SetGUIPanelVisibility ( p->GetMainSlicerWindow()->GetMainPanelVisibility() );
            this->SetLayoutMenubuttonValueToCurrentLayout ();
            }
          else if ( !strcmp ( whichLayout, "Toggle GUI panel L/R"))
            {
            int v = p->GetMainSlicerWindow()->GetViewPanelPosition();
            p->GetMainSlicerWindow()->SetViewPanelPosition ( !v );
            layout->SetGUIPanelLR ( p->GetMainSlicerWindow()->GetViewPanelPosition() );
            this->SetLayoutMenubuttonValueToCurrentLayout();
            }
          }
        else if (this->LoadSceneIconButton!= NULL &&  menu == this->LoadSceneIconButton->GetMenu() )
          {
          const char *thingToDo = this->LoadSceneIconButton->GetValue();
          if ( !strcmp ( thingToDo, "Load scene"))
            {
            p->ProcessLoadSceneCommand();
            this->LoadSceneIconButton->SetValue ( "");
            }
          else if (!strcmp (thingToDo, "Import scene"))
            {
            p->ProcessImportSceneCommand();
            this->LoadSceneIconButton->SetValue ("");
            }
          else if ( !strcmp ( thingToDo, "Add data" ))
            {
            p->ProcessAddDataCommand();
            this->LoadSceneIconButton->SetValue ("");
            }
          }
        }
      }
    }
}




//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::SetLayoutMenubuttonValueToLayout (int layout)
{
  //--- sets the layout dropdown menu selection to match layout value
  if ( this->ChooseLayoutIconMenuButton->GetMenu() != NULL )
    {
    switch ( layout )
      {
      case vtkMRMLLayoutNode::SlicerLayoutConventionalView:
        this->ChooseLayoutIconMenuButton->SetValue ( "Conventional layout" );
        break;
      case vtkMRMLLayoutNode::SlicerLayoutFourUpView:
        this->ChooseLayoutIconMenuButton->SetValue ( "Four-up layout" );
        break;
      case vtkMRMLLayoutNode::SlicerLayoutOneUp3DView:
        this->ChooseLayoutIconMenuButton->SetValue ( "3D only layout" );
        break;
      case vtkMRMLLayoutNode::SlicerLayoutOneUpRedSliceView:
        this->ChooseLayoutIconMenuButton->SetValue ( "Red slice only layout" );
        break;
      case vtkMRMLLayoutNode::SlicerLayoutOneUpYellowSliceView:
        this->ChooseLayoutIconMenuButton->SetValue ( "Yellow slice only layout" );
        break;
      case vtkMRMLLayoutNode::SlicerLayoutOneUpGreenSliceView:
        this->ChooseLayoutIconMenuButton->SetValue ( "Green slice only layout" );
        break;
      case vtkMRMLLayoutNode::SlicerLayoutOneUpSliceView:
        this->ChooseLayoutIconMenuButton->SetValue ( "Red slice only layout" );
        break;
      case vtkMRMLLayoutNode::SlicerLayoutTabbed3DView:
        this->ChooseLayoutIconMenuButton->SetValue ( "Tabbed 3D layout" );
        break;
      case vtkMRMLLayoutNode::SlicerLayoutTabbedSliceView:
        this->ChooseLayoutIconMenuButton->SetValue ( "Tabbed slice layout" );
        break;
      case vtkMRMLLayoutNode::SlicerLayoutCompareView:
        this->ChooseLayoutIconMenuButton->SetValue ( "Compare layout" );
        break;
      case vtkMRMLLayoutNode::SlicerLayoutLightboxView:              
        this->ChooseLayoutIconMenuButton->SetValue ( "Lightbox layout" );
        break;
      default:
        break;
      }
    }
}


//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::SetLayoutMenubuttonValueToCurrentLayout ()
{
  if ( this->GetApplication() != NULL )
    {
    if ( this->GetApplicationGUI()->GetGUILayoutNode() != NULL )
      {
      int layout = this->GetApplicationGUI()->GetGUILayoutNode()->GetViewArrangement ();
      this->SetLayoutMenubuttonValueToLayout ( layout );
      }
    }
}


//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::ResumeViewRockOrSpin ( int mode )
{
  if ( this->ApplicationGUI != NULL )
    {
    vtkSlicerApplicationGUI *p = vtkSlicerApplicationGUI::SafeDownCast( this->GetApplicationGUI ( ));
    if ( p->GetViewControlGUI() != NULL )
      {
      if ( p->GetViewControlGUI()->GetActiveView() != NULL )
        {
        p->GetViewControlGUI()->GetActiveView()->SetAnimationMode( mode );
        }
      }
    }
}



//---------------------------------------------------------------------------
int vtkSlicerToolbarGUI::StopViewRockOrSpin ( )
{
  if ( this->ApplicationGUI != NULL )
    {
    vtkSlicerApplicationGUI *p = vtkSlicerApplicationGUI::SafeDownCast( this->GetApplicationGUI ( ));
    if ( p->GetViewControlGUI() != NULL )
      {
      if ( p->GetViewControlGUI()->GetActiveView() != NULL )
        {
        int mode = p->GetViewControlGUI()->GetActiveView()->GetAnimationMode();
        if ( mode == vtkMRMLViewNode::Rock || mode == vtkMRMLViewNode::Spin )
          {
          p->GetViewControlGUI()->GetActiveView()->SetAnimationMode ( vtkMRMLViewNode::Off );
          }
        return ( mode );
        }
      }
    }
  return ( 0 );
}



//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::ProcessLogicEvents ( vtkObject *caller,
                                            unsigned long event, void *callData )
{
    // Fill in}
}




//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::UpdateLayoutMenu()
{
  if ( this->ApplicationGUI == NULL )
    {
    return;
    }
  vtkSlicerApplicationGUI *appGUI = vtkSlicerApplicationGUI::SafeDownCast ( this->GetApplicationGUI() );
  
  // has the layout changed? Make the GUI track
  //--- gui's current layout
  const char *newLayout = appGUI->GetCurrentLayoutStringName ( );
  const char *guiLayout = this->ChooseLayoutIconMenuButton->GetValue();

  if ( newLayout != NULL )
    {
    if ( (strcmp ( guiLayout, newLayout ) ) != 0 )
      {
      //--- set menu to match node
      this->SetLayoutMenubuttonValueToCurrentLayout ();
      }
    }
}


//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::ProcessMRMLEvents ( vtkObject *caller,
                                           unsigned long event, void *callData )
{
  std::cout << "vtkSlicerToolbarGUI::ProcessMRMLEvents: got event " << event << "(modified = " << vtkCommand::ModifiedEvent << ")" << endl;

  // check for a change on the selection node regarding the mouse interaction mode
  vtkMRMLInteractionNode *interactionNode = this->GetInteractionNode();

  // has the interaction mode changed?
  if (interactionNode == NULL)
    {
    std::cout << "vtkSlicerToolbarGUI::ProcessMRMLEvents: interaction node is null\n";
    return;
    }
  if ( vtkMRMLInteractionNode::SafeDownCast(caller) == interactionNode && event == vtkCommand::ModifiedEvent)
    {
    std::cout << "The selection node changed\n";
    int mode = interactionNode->GetCurrentInteractionMode();
    switch (mode)
      {
      case vtkMRMLInteractionNode::PickManipulate:
        this->MousePickButton->SelectedStateOn();
        break;
      case vtkMRMLInteractionNode::Place:
        this->MousePlaceButton->SelectedStateOn();
        break;
      case vtkMRMLInteractionNode::ViewTransform:
        this->MouseTransformViewButton->SelectedStateOn();
        break;
      default:
        break;
      }
    }
}

//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::Enter ( )
{
    // Fill in
}

//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::Exit ( )
{
    // Fill in
}


//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::SetApplicationGUI ( vtkSlicerApplicationGUI *appGUI )
{
  this->ApplicationGUI = appGUI;
}







//---------------------------------------------------------------------------
void vtkSlicerToolbarGUI::BuildGUI ( )
{
  //
  //--- toolbars
  //
  //--- configure the window's main toolbarset.
  vtkSlicerApplicationGUI *p = this->GetApplicationGUI ( );
  vtkSlicerApplication *app = vtkSlicerApplication::SafeDownCast( p->GetApplication() );
  vtkSlicerWindow *win = p->GetMainSlicerWindow();

  vtkKWToolbarSet *tbs = win->GetMainToolbarSet();
  tbs->SetToolbarsWidgetsAspect ( vtkKWToolbar::WidgetsAspectUnChanged );
  tbs->SetTopSeparatorVisibility ( 0 );
  tbs->SetBottomSeparatorVisibility ( 0 );

  //--- configure toolbars
  vtkKWToolbar *mtb = this->GetModulesToolbar ( );
  mtb->SetParent ( tbs->GetToolbarsFrame ( ) );
  mtb->Create();
  mtb->SetWidgetsFlatAdditionalPadX ( 0 );
  mtb->SetWidgetsFlatAdditionalPadY ( 0 );
  mtb->ResizableOff ( );
  mtb->SetReliefToGroove ( );
  mtb->SetWidgetsPadX ( 3 );
  mtb->SetWidgetsPadY ( 2 );

  vtkKWToolbar *ltb = this->GetLoadSaveToolbar ( );
  ltb->SetName("Load/Save");
  ltb->SetParent ( tbs->GetToolbarsFrame ( ) );
  ltb->Create();
  ltb->SetWidgetsFlatAdditionalPadX ( 0 );
  ltb->SetWidgetsFlatAdditionalPadY ( 0 );
  ltb->ResizableOff ( );
  ltb->SetReliefToGroove ( );
  ltb->SetWidgetsPadX ( 3 );
  ltb->SetWidgetsPadY ( 2 );

  vtkKWToolbar *utb = this->GetUtilitiesToolbar ( );
  utb->SetName ("Utilities" );
  utb->SetParent ( tbs->GetToolbarsFrame() );
  utb->Create();
  utb->SetWidgetsFlatAdditionalPadX(0);
  utb->SetWidgetsFlatAdditionalPadY ( 0 );
  utb->ResizableOff();
  utb->SetReliefToGroove();
  utb->SetWidgetsPadX ( 3 );
  utb->SetWidgetsPadY ( 2 );  

  vtkKWToolbar *vtb = this->GetViewToolbar ( );
  vtb->SetName("View");
  vtb->SetParent ( tbs->GetToolbarsFrame ( ) );
  vtb->Create();
  vtb->SetWidgetsFlatAdditionalPadX ( 0 );
  vtb->SetWidgetsFlatAdditionalPadY ( 0 );
  vtb->ResizableOff ( );
  vtb->SetReliefToGroove ( );
  vtb->SetWidgetsPadX ( 3 );
  vtb->SetWidgetsPadY ( 2 );

  vtkKWToolbar *urtb = this->GetUndoRedoToolbar ( );
  urtb->SetName("Undo/Redo");
  urtb->SetParent ( tbs->GetToolbarsFrame ( ) );
  urtb->Create();
  urtb->SetWidgetsFlatAdditionalPadX ( 0 );
  urtb->SetWidgetsFlatAdditionalPadY ( 0 );
  urtb->ResizableOff ( );
  urtb->SetReliefToGroove ( );
  urtb->SetWidgetsPadX ( 3 );
  urtb->SetWidgetsPadY ( 2 );
  
  vtkKWToolbar *mmtb = this->GetInteractionModeToolbar ( );
  mmtb->SetName("Mouse Mode");
  mmtb->SetParent ( tbs->GetToolbarsFrame ( ) );
  mmtb->Create();
  mmtb->SetWidgetsFlatAdditionalPadX ( 0 );
  mmtb->SetWidgetsFlatAdditionalPadY ( 0 );
  mmtb->ResizableOff ( );
  mmtb->SetReliefToGroove ( );
  mmtb->SetWidgetsPadX ( 3 );
  mmtb->SetWidgetsPadY ( 2 );
        
  //--- and add toolbars to the window's main toolbar set.        
  tbs->AddToolbar ( this->GetLoadSaveToolbar() );
  tbs->AddToolbar ( this->GetModulesToolbar() );
//  tbs->AddToolbar ( this->GetUndoRedoToolbar () );
  tbs->AddToolbar ( this->GetUtilitiesToolbar() );
  tbs->AddToolbar ( this->GetViewToolbar() );
  tbs->AddToolbar ( this->GetInteractionModeToolbar() );
        
  //
  //--- create icons and the labels that display them and add to toolbar
  //
  // load scene icon
  int index;
  this->LoadSceneIconButton->SetParent ( ltb->GetFrame ( ) );
  this->LoadSceneIconButton->Create();
  this->LoadSceneIconButton->SetReliefToFlat ( );
  this->LoadSceneIconButton->SetBorderWidth ( 0 );
  this->LoadSceneIconButton->IndicatorVisibilityOff();
  this->LoadSceneIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetLoadSceneIcon( ) );
  this->LoadSceneIconButton->SetBalloonHelpString ( "Load or import a MRML scene, or add data.");
  this->LoadSceneIconButton->GetMenu()->DeleteAllItems();
  this->LoadSceneIconButton->GetMenu()->AddRadioButton ( "Load scene" );
  index = this->LoadSceneIconButton->GetMenu()->GetIndexOfItem ( "Load scene");
  this->LoadSceneIconButton->GetMenu()->SetItemIndicatorVisibility ( index, 0 );
  this->LoadSceneIconButton->GetMenu()->AddRadioButton ( "Import scene" );
  index = this->LoadSceneIconButton->GetMenu()->GetIndexOfItem ( "Import scene");
  this->LoadSceneIconButton->GetMenu()->SetItemIndicatorVisibility ( index, 0 );
  this->LoadSceneIconButton->GetMenu()->AddRadioButton ( "Add data" );
  index = this->LoadSceneIconButton->GetMenu()->GetIndexOfItem ( "Add data");
  this->LoadSceneIconButton->GetMenu()->SetItemIndicatorVisibility ( index, 0 );
  this->LoadSceneIconButton->GetMenu()->AddSeparator ( );
  this->LoadSceneIconButton->GetMenu()->AddCommand ("close");  
  ltb->AddWidget ( this->LoadSceneIconButton );

  // save scene icon
  this->SaveSceneIconButton->SetParent ( ltb->GetFrame ( ));
  this->SaveSceneIconButton->Create ( );
  this->SaveSceneIconButton->SetReliefToFlat ( );
  this->SaveSceneIconButton->SetBorderWidth ( 0 );
  this->SaveSceneIconButton->SetOverReliefToNone ( );
  this->SaveSceneIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetSaveSceneIcon( ) );
  this->SaveSceneIconButton->SetBalloonHelpString ( "Save a MRML scene or data to a file.");
  ltb->AddWidget ( this->SaveSceneIconButton );

  // build module choose gui here.
    // Build the Module Choose GUI in the Modules toolbar.
  if (this->GetModuleChooseGUI() != NULL )
    {
    this->ModuleChooseGUI->SetApplicationGUI ( p );
    this->ModuleChooseGUI->SetApplication ( app );
    this->ModuleChooseGUI->BuildGUI( mtb );
    }

  // home icon
  this->HomeIconButton->SetParent ( mtb->GetFrame ( ));
  this->HomeIconButton->Create ( );
  this->HomeIconButton->SetReliefToFlat ( );
  this->HomeIconButton->SetBorderWidth ( 0 );
  this->HomeIconButton->SetOverReliefToNone ( );
  this->HomeIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetHomeIcon( ) );
  this->HomeIconButton->SetBalloonHelpString ( "Home module (set home module with Ctrl+h)" );
  mtb->AddWidget ( this->HomeIconButton );

  // data module icon
  this->DataIconButton->SetParent ( mtb->GetFrame ( ));
  this->DataIconButton->Create ( );
  this->DataIconButton->SetReliefToFlat ( );
  this->DataIconButton->SetBorderWidth ( 0 );
  this->DataIconButton->SetOverReliefToNone ( );
  this->DataIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetDataIcon ( ) );
  this->DataIconButton->SetBalloonHelpString ( "Data");
  mtb->AddWidget ( this->DataIconButton );

  // volume module icon
  this->VolumeIconButton->SetParent ( mtb->GetFrame ( ));
  this->VolumeIconButton->Create ( );
  this->VolumeIconButton->SetReliefToFlat ( );
  this->VolumeIconButton->SetBorderWidth ( 0 );
  this->VolumeIconButton->SetOverReliefToNone ( );
  this->VolumeIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetVolumeIcon ( ));
  this->VolumeIconButton->SetBalloonHelpString ( "Volumes");
  mtb->AddWidget ( this->VolumeIconButton );

  // models module icon
  this->ModelIconButton->SetParent (mtb->GetFrame ( ) );
  this->ModelIconButton->Create ( );
  this->ModelIconButton->SetReliefToFlat ( );
  this->ModelIconButton->SetBorderWidth ( 0 );
  this->ModelIconButton->SetOverReliefToNone ( );
  this->ModelIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetModelIcon ( ) );
  this->ModelIconButton->SetBalloonHelpString ( "Models");
  mtb->AddWidget ( this->ModelIconButton );

  // transforms module icon
  this->TransformIconButton->SetParent ( mtb->GetFrame ( ) );
  this->TransformIconButton->Create ( );
  this->TransformIconButton->SetReliefToFlat ( );
  this->TransformIconButton->SetBorderWidth ( 0 );
  this->TransformIconButton->SetOverReliefToNone ( );
  this->TransformIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetTransformIcon ( ) );
  this->TransformIconButton->SetBalloonHelpString ( "Transforms");
  mtb->AddWidget ( this->TransformIconButton );

  // fiducial utility icon
  this->FiducialsIconButton->SetParent ( mtb->GetFrame ( ) );
  this->FiducialsIconButton->Create ( );
  this->FiducialsIconButton->SetReliefToFlat ( );
  this->FiducialsIconButton->SetBorderWidth ( 0 );
  this->FiducialsIconButton->SetOverReliefToNone ( );
  this->FiducialsIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetFiducialsIcon ( ) );
  this->FiducialsIconButton->SetBalloonHelpString ( "Fiducials");
  mtb->AddWidget ( this->FiducialsIconButton );

  // editor module icon

  this->EditorToolboxIconButton->SetParent ( mtb->GetFrame ( ) );
  this->EditorToolboxIconButton->Create ( );
  this->EditorToolboxIconButton->SetReliefToFlat ( );
  this->EditorToolboxIconButton->SetBorderWidth ( 0 );
  this->EditorToolboxIconButton->SetOverReliefToNone ( );
  this->EditorToolboxIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetEditorToolboxIcon ( ) );
  this->EditorToolboxIconButton->SetBalloonHelpString ( "EditBox (or use keyboard 'space')");        
  mtb->AddWidget ( this->EditorToolboxIconButton );

  // editor module icon
  this->EditorIconButton->SetParent ( mtb->GetFrame ( ) );
  this->EditorIconButton->Create ( );
  this->EditorIconButton->SetReliefToFlat ( );
  this->EditorIconButton->SetBorderWidth ( 0 );
  this->EditorIconButton->SetOverReliefToNone ( );
  this->EditorIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetEditorIcon ( ) );
  this->EditorIconButton->SetBalloonHelpString ( "Editor");        
  mtb->AddWidget ( this->EditorIconButton );

  // measurements module icon
/*
  this->MeasurementsIconButton->SetParent ( mtb->GetFrame ( ) );
  this->MeasurementsIconButton->Create ( );
  this->MeasurementsIconButton->SetReliefToFlat ( );
  this->MeasurementsIconButton->SetBorderWidth ( 0 );
  this->MeasurementsIconButton->SetOverReliefToNone ( );
  this->MeasurementsIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetMeasurementsIcon ( ) );
  this->MeasurementsIconButton->SetBalloonHelpString ( "Measurements (not yet available)");        
  mtb->AddWidget ( this->MeasurementsIconButton );
*/
  
  // color utility icon
  this->ColorIconButton->SetParent ( mtb->GetFrame ( ) );
  this->ColorIconButton->Create ( );
  this->ColorIconButton->SetReliefToFlat ( );
  this->ColorIconButton->SetBorderWidth ( 0 );
  this->ColorIconButton->SetOverReliefToNone ( );
  this->ColorIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetColorIcon ( ) );
  this->ColorIconButton->SetBalloonHelpString ( "Colors");
  mtb->AddWidget ( this->ColorIconButton );

  // undo icon
  this->UndoIconButton->SetParent ( urtb->GetFrame ( ) );
  this->UndoIconButton->Create ( );
  this->UndoIconButton->SetReliefToFlat ( );
  this->UndoIconButton->SetBorderWidth ( 0 );
  this->UndoIconButton->SetOverReliefToNone ( );
  this->UndoIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetUndoIcon ( ) );
  this->UndoIconButton->SetBalloonHelpString ( "Undo (or use keyboard Ctrl+Z)");
  urtb->AddWidget ( this->UndoIconButton );

  // redo icon
  this->RedoIconButton->SetParent ( urtb->GetFrame ( ) );
  this->RedoIconButton->Create ( );
  this->RedoIconButton->SetReliefToFlat ( );
  this->RedoIconButton->SetBorderWidth ( 0 );
  this->RedoIconButton->SetOverReliefToNone ( );
  this->RedoIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetRedoIcon ( ) );
  this->RedoIconButton->SetBalloonHelpString ( "Redo (or use keyboard Ctrl+Y)");
  urtb->AddWidget ( this->RedoIconButton );

  // Utilities toolbar.
  this->ScreenShotIconButton->SetParent ( utb->GetFrame() );
  this->ScreenShotIconButton->Create();
  this->ScreenShotIconButton->SetReliefToFlat();
  this->ScreenShotIconButton->SetBorderWidth (0);
  this->ScreenShotIconButton->SetOverReliefToNone ( );
  this->ScreenShotIconButton->SetImageToIcon ( this->SlicerToolbarIcons->GetScreenShotIcon() );
  this->ScreenShotIconButton->SetBalloonHelpString ( "Raise options for capturing one or more screen shots." );  
  utb->AddWidget ( this->ScreenShotIconButton );

  // Layout choose menu
  const char *imageName;
  this->ChooseLayoutIconMenuButton->SetParent ( vtb->GetFrame ( ) );
  this->ChooseLayoutIconMenuButton->Create ( );
  this->ChooseLayoutIconMenuButton->SetReliefToFlat ( );
  this->ChooseLayoutIconMenuButton->SetBorderWidth ( 0 );
  this->ChooseLayoutIconMenuButton->IndicatorVisibilityOff ( );
  this->ChooseLayoutIconMenuButton->SetImageToIcon ( this->SlicerToolbarIcons->GetChooseLayoutIcon ( ) );
  this->ChooseLayoutIconMenuButton->SetBalloonHelpString ( "Choose among layouts for the 3D and Slice viewers" );
  this->ChooseLayoutIconMenuButton->GetMenu()->DeleteAllItems ( );

  this->ChooseLayoutIconMenuButton->GetMenu()->AddRadioButton ( "Conventional layout" );
  index = this->ChooseLayoutIconMenuButton->GetMenu()->GetIndexOfItem ( "Conventional layout");
  imageName = "SlicerConventionalLayoutImage";
  vtkKWTkUtilities::UpdatePhotoFromIcon ( this->GetApplication(), imageName, this->SlicerToolbarIcons->GetConventionalViewIcon(), 0 );
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemImage ( index, imageName);
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemCompoundModeToLeft ( index );
//  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemVariableValueAsInt ("Conventional layout", vtkMRMLLayoutNode::SlicerLayoutConventionalView );

  this->ChooseLayoutIconMenuButton->GetMenu()->AddRadioButton ( "Four-up layout");
  index = this->ChooseLayoutIconMenuButton->GetMenu()->GetIndexOfItem ( "Four-up layout");
  imageName = "SlicerFourUpLayoutImage";
  vtkKWTkUtilities::UpdatePhotoFromIcon ( this->GetApplication(), imageName, this->SlicerToolbarIcons->GetFourUpViewIcon(), 0 );
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemImage ( index, imageName);
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemCompoundModeToLeft ( index );
//  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemVariableValueAsInt ( "Four-up layout", vtkMRMLLayoutNode::SlicerLayoutFourUpView );

  this->ChooseLayoutIconMenuButton->GetMenu()->AddRadioButton ( "3D only layout");
  index = this->ChooseLayoutIconMenuButton->GetMenu()->GetIndexOfItem ( "3D only layout");
  imageName = "Slicer3DOnlyLayoutImage";
  vtkKWTkUtilities::UpdatePhotoFromIcon ( this->GetApplication(), imageName, this->SlicerToolbarIcons->GetOneUp3DViewIcon(), 0 );
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemImage ( index, imageName);
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemCompoundModeToLeft ( index );
//  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemVariableValueAsInt ( "3D only layout", vtkMRMLLayoutNode::SlicerLayoutOneUp3DView );

  this->ChooseLayoutIconMenuButton->GetMenu()->AddRadioButton ( "Red slice only layout");
  index = this->ChooseLayoutIconMenuButton->GetMenu()->GetIndexOfItem ( "Red slice only layout");
  imageName = "SlicerRedSliceOnlyLayoutImage";
  vtkKWTkUtilities::UpdatePhotoFromIcon ( this->GetApplication(), imageName, this->SlicerToolbarIcons->GetOneUpRedSliceViewIcon(), 0 );
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemImage ( index, imageName);
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemCompoundModeToLeft ( index );
//  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemVariableValueAsInt ( "Red slice only layout", vtkMRMLLayoutNode::SlicerLayoutOneUpRedSliceView);

  this->ChooseLayoutIconMenuButton->GetMenu()->AddRadioButton ( "Yellow slice only layout");
  index = this->ChooseLayoutIconMenuButton->GetMenu()->GetIndexOfItem ( "Yellow slice only layout");
  imageName = "SlicerYellowSliceOnlyLayoutImage";
  vtkKWTkUtilities::UpdatePhotoFromIcon ( this->GetApplication(), imageName, this->SlicerToolbarIcons->GetOneUpYellowSliceViewIcon(), 0 );
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemImage ( index, imageName);
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemCompoundModeToLeft ( index );
//  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemVariableValueAsInt ("Yellow slice only layout", vtkMRMLLayoutNode::SlicerLayoutOneUpYellowSliceView);

  this->ChooseLayoutIconMenuButton->GetMenu()->AddRadioButton ( "Green slice only layout" );
  index = this->ChooseLayoutIconMenuButton->GetMenu()->GetIndexOfItem ( "Green slice only layout");
  imageName = "SlicerGreenSliceOnlyLayoutImage";
  vtkKWTkUtilities::UpdatePhotoFromIcon ( this->GetApplication(), imageName, this->SlicerToolbarIcons->GetOneUpGreenSliceViewIcon(), 0 );
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemImage ( index, imageName);
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemCompoundModeToLeft ( index );
//  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemVariableValueAsInt ( "Green slice only layout", vtkMRMLLayoutNode::SlicerLayoutOneUpGreenSliceView);

  this->ChooseLayoutIconMenuButton->GetMenu()->AddRadioButton ( "Tabbed 3D layout" );
  index = this->ChooseLayoutIconMenuButton->GetMenu()->GetIndexOfItem ( "Tabbed 3D layout");
  imageName = "SlicerTabbed3DLayoutImage";
  vtkKWTkUtilities::UpdatePhotoFromIcon ( this->GetApplication(), imageName, this->SlicerToolbarIcons->GetTabbed3DViewIcon(), 0 );
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemImage ( index, imageName);
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemCompoundModeToLeft ( index );
//  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemVariableValueAsInt ( "Tabbed 3D layout", vtkMRMLLayoutNode::SlicerLayoutTabbed3DView);

  this->ChooseLayoutIconMenuButton->GetMenu()->AddRadioButton ( "Tabbed slice layout" );
  index = this->ChooseLayoutIconMenuButton->GetMenu()->GetIndexOfItem ( "Tabbed slice layout");
  imageName = "SlicerTabbedSliceLayoutImage";
  vtkKWTkUtilities::UpdatePhotoFromIcon ( this->GetApplication(), imageName, this->SlicerToolbarIcons->GetTabbedSliceViewIcon(), 0 );
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemImage ( index, imageName);
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemCompoundModeToLeft ( index );
//  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemVariableValueAsInt ( "Tabbed slice layout", vtkMRMLLayoutNode::SlicerLayoutTabbedSliceView);

  this->ChooseLayoutIconMenuButton->GetMenu()->AddRadioButton ("Compare layout");
  index = this->ChooseLayoutIconMenuButton->GetMenu()->GetIndexOfItem ("Compare layout");
  imageName = "SlicerCompareViewLayoutImage";
  vtkKWTkUtilities::UpdatePhotoFromIcon ( this->GetApplication(), imageName, this->SlicerToolbarIcons->GetCompareViewIcon(), 0);
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemImage ( index, imageName );
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemCompoundModeToLeft ( index );


  this->ChooseLayoutIconMenuButton->GetMenu()->AddRadioButton ( "Toggle GUI panel visibility" );
  index = this->ChooseLayoutIconMenuButton->GetMenu()->GetIndexOfItem ( "Toggle GUI panel visibility");
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemIndicatorVisibility ( index, 0 );


  this->ChooseLayoutIconMenuButton->GetMenu()->AddRadioButton ( "Toggle GUI panel L/R" );
  index = this->ChooseLayoutIconMenuButton->GetMenu()->GetIndexOfItem ( "Toggle GUI panel L/R");
  this->ChooseLayoutIconMenuButton->GetMenu()->SetItemIndicatorVisibility ( index, 0 );
  
  this->ChooseLayoutIconMenuButton->GetMenu()->AddSeparator ( );
  this->ChooseLayoutIconMenuButton->GetMenu()->AddCommand ("close");  
  this->ChooseLayoutIconMenuButton->SetBinding ( "<Button-1>", this, "StopViewRockOrSpin" );

    ////--- Pop-up frame for custom NXM lightbox configuration
    this->CompareViewBoxTopLevel = vtkKWTopLevel::New ( );
    this->CompareViewBoxTopLevel->SetApplication ( app );
    this->CompareViewBoxTopLevel->SetMasterWindow ( this->ChooseLayoutIconMenuButton );
    this->CompareViewBoxTopLevel->Create ( );
    this->CompareViewBoxTopLevel->HideDecorationOn ( );
    this->CompareViewBoxTopLevel->Withdraw ( );
    this->CompareViewBoxTopLevel->SetBorderWidth ( 2 );
    this->CompareViewBoxTopLevel->SetReliefToGroove ( );

    //--- create frames to display when compare viewer is selected
    vtkKWFrameWithLabel *viewerConfigFrame = vtkKWFrameWithLabel::New ( );
    viewerConfigFrame->SetParent( this->CompareViewBoxTopLevel );
    viewerConfigFrame->Create ( );
    viewerConfigFrame->SetLabelText ( "Compare viewer options" );
    this->Script ( "pack %s -side top -anchor w -padx 4 -pady 2 -fill x -fill y -expand n", viewerConfigFrame->GetWidgetName ( ) );   
    this->Script ( "grid columnconfigure %s 0 -weight 0", viewerConfigFrame->GetWidgetName() );
    this->Script ( "grid columnconfigure %s 1 -weight 0", viewerConfigFrame->GetWidgetName() );

    vtkKWFrame *applyCancelFrame = vtkKWFrame::New();
    applyCancelFrame->SetParent ( this->CompareViewBoxTopLevel );
    applyCancelFrame->Create();
    this->Script ( "pack %s -side top -anchor w -padx 2 -pady 2 -fill x -fill y -expand n", applyCancelFrame->GetWidgetName ( ) );   
    this->Script ( "grid columnconfigure %s 0 -weight 0", applyCancelFrame->GetWidgetName() );
    this->Script ( "grid columnconfigure %s 1 -weight 0", applyCancelFrame->GetWidgetName() );

    //--- choose how many compare viewers are arrayed horizontally and vertically
    vtkKWLabel *rowsLabel = vtkKWLabel::New();
    rowsLabel->SetParent ( viewerConfigFrame->GetFrame() );
    rowsLabel->Create ( );
    rowsLabel->SetText ( "Number of compare viewers:" );
    this->CompareViewBoxRowEntry = vtkKWEntry::New ( );
    this->CompareViewBoxRowEntry->SetParent ( viewerConfigFrame->GetFrame() );
    this->CompareViewBoxRowEntry->Create ( );
    this->CompareViewBoxRowEntry->SetValueAsInt (2);
    this->CompareViewBoxRowEntry->SetWidth ( 3 );
    //--- grid compare viewer configuration widgets
    this->Script ( "grid %s -row 0 -column 0 -sticky e -padx 2 -pady 4", rowsLabel->GetWidgetName());
    this->Script ( "grid %s -row 0 -column 1 -sticky w -padx 2 -pady 4", this->CompareViewBoxRowEntry->GetWidgetName() );

    vtkKWLabel *l2 = vtkKWLabel::New();
    l2->SetParent ( viewerConfigFrame->GetFrame() );
    l2->Create ( );
    l2->SetText ( "Lightbox rows in each viewer:" );
    this->CompareViewLightboxRowEntry = vtkKWEntry::New();
    this->CompareViewLightboxRowEntry->SetParent ( viewerConfigFrame->GetFrame() );
    this->CompareViewLightboxRowEntry->Create();
    this->CompareViewLightboxRowEntry->SetWidth ( 3 );
    this->CompareViewLightboxRowEntry->SetValueAsInt ( 1 );
    vtkKWLabel *l3 = vtkKWLabel::New();
    l3->SetParent ( viewerConfigFrame->GetFrame() );
    l3->Create ( );
    l3->SetText ( "Lightbox columns in each viewer:" );
    this->CompareViewLightboxColumnEntry = vtkKWEntry::New();
    this->CompareViewLightboxColumnEntry->SetParent ( viewerConfigFrame->GetFrame() );
    this->CompareViewLightboxColumnEntry->Create();    
    this->CompareViewLightboxColumnEntry->SetWidth ( 3 );
    this->CompareViewLightboxColumnEntry->SetValueAsInt ( 6 );
    //--- grid up lightbox configuration
    this->Script ( "grid %s -row 1 -column 0 -padx 2 -pady 4 -sticky e", l2->GetWidgetName());
    this->Script ( "grid %s -row 1 -column 1 -padx 2 -pady 4 -sticky w", this->CompareViewLightboxRowEntry->GetWidgetName() );
    this->Script ( "grid %s -row 2 -column 0 -padx 2 -pady 4 -sticky e", l3->GetWidgetName());
    this->Script ( "grid %s -row 2 -column 1 -padx 2 -pady 4 -sticky w", this->CompareViewLightboxColumnEntry->GetWidgetName() );

    //--- apply or cancel the compare view configuration.
    this->CompareViewBoxApplyButton = vtkKWPushButton::New ( );
    this->CompareViewBoxApplyButton->SetParent ( applyCancelFrame);
    this->CompareViewBoxApplyButton->Create ( );
    this->CompareViewBoxApplyButton->SetText ("Apply");    
    this->CompareViewBoxApplyButton->SetWidth ( 10 );
    vtkKWPushButton *b = vtkKWPushButton::New();
    b->SetParent ( applyCancelFrame );
    b->Create();
    b->SetText ( "Cancel");
    b->SetWidth ( 10 );
    b->SetBinding ( "<Button-1>", this,  "HideCompareViewCustomLayoutFrame");
    this->Script ( "grid %s -row 0 -column 0 -padx 2 -pady 8 -sticky ew", b->GetWidgetName() );
    this->Script ( "grid %s -row 0 -column 1 -padx 2 -pady 8 -sticky ew", this->CompareViewBoxApplyButton->GetWidgetName() );

    // delete temporary stuff
    l2->Delete();
    l3->Delete();
    b->Delete();
    applyCancelFrame->Delete();
    rowsLabel->Delete();
    viewerConfigFrame->Delete();

    vtb->AddWidget (this->ChooseLayoutIconMenuButton );

  //---
  //--- Mouse mode toolbar
  //---


  int mouseMode = vtkMRMLInteractionNode::ViewTransform;

  // try to get the mouse interaction mode from the mrml scene
  vtkMRMLInteractionNode *interactionNode = NULL;
  if (this->ApplicationLogic != NULL)
    {
    interactionNode =  this->ApplicationLogic->GetInteractionNode();
    if (interactionNode != NULL)
      {
      mouseMode = interactionNode->GetCurrentInteractionMode();
      }
    }
  else 
    { 
    vtkDebugMacro ("MRML Scene not set yet, not getting mouse interaction mode, using default of transform\n");
    mouseMode = vtkMRMLInteractionNode::ViewTransform;
    }

  this->MousePickButton->SetParent (mmtb->GetFrame() );
  this->MousePickButton->Create();
  this->MousePickButton->SetValueAsInt ( vtkMRMLInteractionNode::PickManipulate );
  this->MousePickButton->SetImageToIcon ( this->SlicerToolbarIcons->GetMousePickOffIcon ( ) );
  this->MousePickButton->SetSelectImageToIcon ( this->SlicerToolbarIcons->GetMousePickOnIcon ( ) );
  this->MousePickButton->SetBalloonHelpString ( "Set the 3DViewer mouse mode to 'pick'" );
  if ( mouseMode == vtkMRMLInteractionNode::PickManipulate)
    {
    this->MousePickButton->SetSelectedState ( 1 );
    }
  else
    {
    this->MousePickButton->SetSelectedState ( 0 );
    }
  mmtb->AddWidget ( this->MousePickButton );
  
  this->MousePlaceButton->SetParent (mmtb->GetFrame() );
  this->MousePlaceButton->Create();
  this->MousePlaceButton->SetValueAsInt ( vtkMRMLInteractionNode::Place );
  this->MousePlaceButton->SetImageToIcon ( this->SlicerToolbarIcons->GetMousePlaceOffIcon ( ) );
  this->MousePlaceButton->SetSelectImageToIcon ( this->SlicerToolbarIcons->GetMousePlaceOnIcon ( ) );
  this->MousePlaceButton->SetBalloonHelpString ( "Set the 3DViewer mouse mode to 'place a new object (like a fiducial point)'" );
  this->MousePlaceButton->SetVariableName ( this->MousePickButton->GetVariableName() );
  if ( mouseMode == vtkMRMLInteractionNode::Place)
    {
    this->MousePlaceButton->SetSelectedState ( 1 );
    }
  else
    {
    this->MousePlaceButton->SetSelectedState ( 0 );
    }
  mmtb->AddWidget ( this->MousePlaceButton );

  this->MouseTransformViewButton->SetParent (mmtb->GetFrame() );
  this->MouseTransformViewButton->Create();
  this->MouseTransformViewButton->SetValueAsInt ( vtkMRMLInteractionNode::ViewTransform );
  this->MouseTransformViewButton->SetImageToIcon ( this->SlicerToolbarIcons->GetMouseTransformViewOffIcon ( ) );
  this->MouseTransformViewButton->SetSelectImageToIcon ( this->SlicerToolbarIcons->GetMouseTransformViewOnIcon ( ) );
  this->MouseTransformViewButton->SetSelectColor ( app->GetSlicerTheme()->GetSlicerColors()->White );
  this->MouseTransformViewButton->SetBalloonHelpString ( "Set the 3DViewer mouse mode to 'transform view'" );
  this->MouseTransformViewButton->SetVariableName ( this->MousePickButton->GetVariableName() );
  if ( mouseMode == vtkMRMLInteractionNode::ViewTransform)
    {
    this->MouseTransformViewButton->SetSelectedState ( 1 );
    }
  else
    {
    this->MouseTransformViewButton->SetSelectedState ( 0 );
    }
  mmtb->AddWidget ( this->MouseTransformViewButton );

  // configure fonts for all widgets that have text.
  this->ReconfigureGUIFonts();

}

void vtkSlicerToolbarGUI::PopUpCompareViewCustomLayoutFrame()
{
  if ( !this->ChooseLayoutIconMenuButton || !this->ChooseLayoutIconMenuButton->IsCreated())
    {
    return;
    }
  /*
  if ( !this->UndoIconButton || !this->UndoIconButton->IsCreated())
    {
    return;
    }
  */
  if ( !this->ColorIconButton || !this->ColorIconButton->IsCreated())
    {
    return;
    }

  // Get the position of the mouse, the position and size of the push button,
  // the size of the scale.

  int x, y, px, py, ph;
  vtkSlicerApplication *app = vtkSlicerApplication::SafeDownCast ( this->GetApplication());
  
  vtkKWTkUtilities::GetMousePointerCoordinates(this->ChooseLayoutIconMenuButton, &x, &y);
  vtkKWTkUtilities::GetWidgetCoordinates(this->ColorIconButton, &px, &py);
//  vtkKWTkUtilities::GetWidgetCoordinates(this->UndoIconButton, &px, &py);
  vtkKWTkUtilities::GetWidgetSize(this->ChooseLayoutIconMenuButton, NULL, &ph);
 
  this->CompareViewBoxTopLevel->SetPosition(px-ph, py+ph);
  app->ProcessPendingEvents();
  this->CompareViewBoxTopLevel->DeIconify();
  this->CompareViewBoxTopLevel->Raise();
}

void vtkSlicerToolbarGUI::HideCompareViewCustomLayoutFrame()
{
    if ( !this->CompareViewBoxTopLevel )
      return;

    this->CompareViewBoxTopLevel->Withdraw();
}
