{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit codeanifrm;

{$mode objfpc}{$H+}

interface

uses
  LCLIntf, LCLType, LMessages, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TfrmAniCode = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    Panel1: TD2Panel;
    Container: TD2Layout;
    Rectangle1: TD2Rectangle;
    Rectangle4: TD2Rectangle;
    Rectangle2: TD2Rectangle;
    Rectangle5: TD2Rectangle;
    Rectangle6: TD2Rectangle;
    Rectangle7: TD2Rectangle;
    Rectangle9: TD2Rectangle;
    Rectangle11: TD2Rectangle;
    radioVert: TD2RadioButton;
    radioHorz: TD2RadioButton;
    radioGrid: TD2RadioButton;
    Button1: TD2Button;
    Button2: TD2Button;
    procedure FormCreate(Sender: TObject);
    procedure radioVertChange(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
  private
    procedure LayoutItem(Item: TD2VisualObject; Animate: boolean = true);
    { Private declarations }
  public
    { Public declarations }
    procedure Layout;
  end;

var
  frmAniCode: TfrmAniCode;

implementation

{$R *.lfm}

var
  LayoutStyle: integer = 0;

procedure TfrmAniCode.LayoutItem(Item: TD2VisualObject; Animate: boolean = true);
var
  Pos: TD2Point;
begin
  if radioVert.IsChecked then
    Pos := d2Point(Container.Width / 2, Item.Index * 60);
  if radioHorz.IsChecked then
    Pos := d2Point(Item.Index * 60, Container.Height / 2);
  if radioGrid.IsChecked then
    Pos := d2Point((Item.Index mod 4) * 60, (Item.Index div 4) * 60);

  if Animate then
  begin
    Item.AnimateFloat('Position.X', Pos.X, 0.5);
    Item.AnimateFloat('Position.Y', Pos.Y, 0.5);
  end
  else
    Item.Position.Point := Pos;
end;

procedure TfrmAniCode.Layout;
var
  i: integer;
begin
  for i := 0 to Container.ChildrenCount - 1 do
    LayoutItem(Container.Children[i].Visual);
end;

procedure TfrmAniCode.FormCreate(Sender: TObject);
begin
  Layout;
end;

procedure TfrmAniCode.radioVertChange(Sender: TObject);
begin
  Layout;
end;

procedure TfrmAniCode.Button1Click(Sender: TObject);
var
  Item: TD2Rectangle;
begin
  Item := TD2Rectangle.Create(Self);
  Item.Parent := Container;
  Item.Opacity := 0; // Make object transparent
  LayoutItem(Item, false); // Layout
  Item.AnimateFloat('Opacity', 1, 0.5); // Animate opacity
end;

procedure TfrmAniCode.Button2Click(Sender: TObject);
begin
  Button2.Enabled := false;
  if Container.ChildrenCount > 0 then
    with Container.Children[Container.ChildrenCount - 1].Visual do // Visual is a just replace of TD2VisualObject()
    begin
      AnimateFloatWait('Opacity', 0, 0.5); // Wait when animation finished
      Free; // Then free object
    end;
  Button2.Enabled := true
end;

end.
