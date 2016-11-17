{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit designform;

  {$mode objfpc}{$H+}

interface

uses
  LResources,  SysUtils, Variants, Classes, db, Graphics, Controls, Forms,
  Dialogs, orca_scene2d, ZConnection, ZDataset;

type

  { TfrmDesigner }

  TfrmDesigner = class(TForm)
    d2SceneDesigner: TD2Scene;
    d2Toolbar: TD2Scene;
    DesignerRoot: TD2Background;
    EditorStore: TD2Rectangle;
    Label2: TD2Label;
    Root2: TD2Background;
    Rectangle1: TD2Rectangle;
    Label1: TD2Label;
    SpeedButton1: TD2SpeedButton;
    SpeedButton2: TD2SpeedButton;
    SpeedButton3: TD2SpeedButton;
    PopupBox1: TD2PopupBox;
    PopupBox2: TD2PopupBox;
    PopupBox3: TD2PopupBox;
    TrackBar1: TD2TrackBar;
    CheckBox1: TD2CheckBox;
    d2SceneInspector: TD2Scene;
    Root1: TD2Layout;
    Inspector1: TD2Inspector;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure SpeedButton3Click(Sender: TObject);
    procedure PopupBox1Change(Sender: TObject);
    procedure PopupBox2Change(Sender: TObject);
    procedure PopupBox3Change(Sender: TObject);
    procedure TrackBar1Change(Sender: TObject);
    procedure CheckBox1Change(Sender: TObject);
  private
    { Private declarations }
    procedure DoChangeSelected(Sender: TObject);
  public
    { Public declarations }
  end;

var
  frmDesigner: TfrmDesigner;

implementation


{$R *.lfm}

procedure TfrmDesigner.FormCreate(Sender: TObject);
begin
  // Set this property at run-time - enable design feature
  d2SceneDesigner.DesignTime := true;
  d2SceneDesigner.DesignChangeSelection := @DoChangeSelected;
end;

procedure TfrmDesigner.Button1Click(Sender: TObject);
begin

end;

procedure TfrmDesigner.Button2Click(Sender: TObject);
begin

end;

procedure TfrmDesigner.SpeedButton3Click(Sender: TObject);
begin
  // Edit selected object Fill and Stroke
  if d2SceneDesigner.Selected is TD2Shape then
  begin
    SelectInDesign(TD2Shape(d2SceneDesigner.Selected).Fill, d2SceneDesigner.Selected);
  end;
end;

procedure TfrmDesigner.PopupBox1Change(Sender: TObject);
begin
  if not (d2SceneDesigner.Selected is TD2Shape) then Exit;
  TD2Shape(d2SceneDesigner.Selected).StrokeDash := TD2StrokeDash(PopupBox1.ItemIndex);
end;

procedure TfrmDesigner.PopupBox2Change(Sender: TObject);
begin
  if not (d2SceneDesigner.Selected is TD2Shape) then Exit;
  TD2Shape(d2SceneDesigner.Selected).StrokeCap := TD2StrokeCap(PopupBox2.ItemIndex);
end;

procedure TfrmDesigner.PopupBox3Change(Sender: TObject);
begin
  if not (d2SceneDesigner.Selected is TD2Shape) then Exit;
  TD2Shape(d2SceneDesigner.Selected).StrokeJoin := TD2StrokeJoin(PopupBox3.ItemIndex);
end;

procedure TfrmDesigner.TrackBar1Change(Sender: TObject);
begin
  if not (d2SceneDesigner.Selected is TD2Shape) then Exit;
  TD2Shape(d2SceneDesigner.Selected).StrokeThickness := TrackBar1.Value;
end;


procedure TfrmDesigner.CheckBox1Change(Sender: TObject);
begin
  d2SceneDesigner.DesignPopupEnabled := CheckBox1.IsChecked;
end;

procedure TfrmDesigner.DoChangeSelected(Sender: TObject);
begin
  Inspector1.SelectedObject := d2SceneDesigner.Selected;
end;

end.
