{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit layoutdemoform;

  {$mode objfpc}{$H+}

interface

uses

  LResources,
  SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs,orca_scene2d,
  ExtCtrls;

type
  TfrmLayout = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    TabControl1: TD2TabControl;
    TabItem1: TD2TabItem;
    Layout1: TD2Layout;
    TabItem2: TD2TabItem;
    Layout2: TD2Layout;
    TabControl2: TD2TabControl;
    TabItem3: TD2TabItem;
    Layout3: TD2Layout;
    TabItem4: TD2TabItem;
    Layout4: TD2Layout;
    Rectangle1: TD2Rectangle;
    ScaledLayout1: TD2ScaledLayout;
    Rectangle2: TD2Rectangle;
    TabItem5: TD2TabItem;
    Layout5: TD2Layout;
    Grid1: TD2GridLayout;
    Rectangle3: TD2Rectangle;
    Rectangle4: TD2Rectangle;
    Rectangle5: TD2Rectangle;
    Rectangle6: TD2Rectangle;
    Rectangle7: TD2Rectangle;
    Rectangle8: TD2Rectangle;
    Rectangle9: TD2Rectangle;
    Rectangle10: TD2Rectangle;
    Rectangle11: TD2Rectangle;
    Rectangle12: TD2Rectangle;
    Rectangle13: TD2Rectangle;
    Rectangle14: TD2Rectangle;
    Rectangle15: TD2Rectangle;
    Rectangle16: TD2Rectangle;
    Rectangle17: TD2Rectangle;
    Rectangle18: TD2Rectangle;
    Rectangle19: TD2Rectangle;
    Rectangle20: TD2Rectangle;
    Rectangle21: TD2Rectangle;
    Rectangle22: TD2Rectangle;
    TabItem6: TD2TabItem;
    Layout6: TD2Layout;
    Rectangle23: TD2Rectangle;
    Label1: TD2Label;
    Nond2Layout1: TD2Nond2Layout;
    Panel1: TPanel;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmLayout: TfrmLayout;

implementation

{$R *.lfm}

end.
