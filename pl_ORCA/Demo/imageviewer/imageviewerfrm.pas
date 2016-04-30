{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit imageviewerfrm;

  {$mode objfpc}{$H+}

interface

uses
  LResources,
  SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs,ExtDlgs, orca_scene2d;

type
  TfrmImageViewer = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Button1: TD2Button;
    Layout1: TD2Layout;
    OpenPictureDialog1: TOpenDialog;
    Label1: TD2Label;
    Label2: TD2Label;
    imageList: TD2HorzImageListBox;
    StyleBox: TD2PopupBox;
    Label3: TD2Label;
    ModernStyle: TD2Resources;
    VistaStyle: TD2Resources;
    imageList2: TD2ImageListBox;
    imageList3: TD2HorzImageListBox;
    Label4: TD2Label;
    ImageViewer1: TD2ImageViewer;
    procedure Button1Click(Sender: TObject);
    procedure imageListChange(Sender: TObject);
    procedure StyleBoxChange(Sender: TObject);
    procedure imageList2Change(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure imageList3Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmImageViewer: TfrmImageViewer;

implementation

{$R *.lfm}

{ TfrmImageViewer =============================================================}

procedure TfrmImageViewer.Button1Click(Sender: TObject);
var
  Dir: string;
begin
  { add folder }
  OpenPictureDialog1.Filter := 'Image files|' + GVarD2DefaultFilterClass.GetFileTypes;
  if OpenPictureDialog1.Execute then
  begin
    Dir := ExtractFilePath(OpenPictureDialog1.FileName);
    imageList.AddFolder(Dir);
    imageList2.AddFolder(Dir);
    imageList3.AddFolder(Dir);
  end;
end;

procedure TfrmImageViewer.imageListChange(Sender: TObject);
begin
  if (imageList.SelectedFileName <> '') then
  begin
    ImageViewer1.Bitmap.LoadThumbnailFromFile(imageList.SelectedFileName, ImageViewer1.Width, ImageViewer1.Height, false);
  end;
end;

procedure TfrmImageViewer.StyleBoxChange(Sender: TObject);
begin
  case StyleBox.ItemIndex of
    0: d2Scene1.Style := nil;
    1: d2Scene1.Style := ModernStyle;
    2: d2Scene1.Style := VistaStyle;
  end;
end;

procedure TfrmImageViewer.imageList2Change(Sender: TObject);
begin
  if (imageList2.SelectedFileName <> '') then
  begin
    ImageViewer1.Bitmap.LoadThumbnailFromFile(imageList2.SelectedFileName, ImageViewer1.Width, ImageViewer1.Height, false);
  end;
end;

procedure TfrmImageViewer.Button2Click(Sender: TObject);
begin
  imageList.Clear;
  imageList2.Clear;
end;

procedure TfrmImageViewer.imageList3Change(Sender: TObject);
begin
  if (imageList3.SelectedFileName <> '') then
  begin
    ImageViewer1.Bitmap.LoadThumbnailFromFile(imageList3.SelectedFileName, ImageViewer1.Width, ImageViewer1.Height, false);
  end;
end;

end.
