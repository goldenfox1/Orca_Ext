{%mainunit ../orca_scene2d.pas}
            

{**********************************************************************
 Package pl_ORCA.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}


//=============== TD2BrushDialog =================================================


constructor TD2BrushDialog.Create(AOwner:TComponent);
begin
  inherited;
  FBrush:=TD2Brush.Create(d2BrushSolid,$FF808080);
  FShowStyles:=[d2BrushNone,d2BrushSolid,d2BrushGradient,d2BrushBitmap,d2BrushResource];
  FShowBrushList:=true;
  FShowMakeResource:=false;
  FTitle:='Brush';
end;

destructor TD2BrushDialog.Destroy;
begin
  FBrush.Free;
  inherited;
end;

function TD2BrushDialog.Execute:boolean;
var
  Dialog:TD2BrushDesign;
  EditBrush:TD2Brush;
begin
  Dialog:=TD2BrushDesign.Create(Application);
  Dialog.brushList.Visible:=ShowBrushList;
  if FComponent <> nil then
    Dialog.Comp:=FComponent
  else
    Dialog.brushList.Visible:=false;

  Dialog.HudWindow1.TextW:=FTitle;
  Dialog.brushTabControl.ItemIndex:=-1;
  Dialog.tabNone.Visible:=d2BrushNone in ShowStyles;
  Dialog.tabSolid.Visible:=d2BrushSolid in ShowStyles;
  Dialog.tabGradient.Visible:=d2BrushGradient in ShowStyles;
  Dialog.tabBitmap.Visible:=d2BrushBitmap in ShowStyles;
  Dialog.tabRes.Visible:=d2BrushResource in ShowStyles;
  Dialog.brushTabControl.Realign;
  Dialog.btnOK.Visible:=true;
  Dialog.btnCancel.Visible:=true;

  Dialog.Brush:=FBrush;
  Dialog.makeResLayout.Visible:=ShowMakeResource;
  Dialog.dsgnRoot.Height:=Dialog.dsgnRoot.Height - 1; // realign
  Dialog.ClientHeight:=Trunc(Dialog.Layout6.Position.Y + Dialog.Layout6.Height + Dialog.HudWindow1.Margins.Top + Dialog.HudWindow1.Margins.Bottom);
  Result:=Dialog.ShowModal=mrOk;
  Dialog.Free;
end;

procedure TD2BrushDialog.SetBrush(const Value:TD2Brush);
begin
  FBrush.Assign(Value);;
end;
           
                                 
