{**********************************************************************
 orca 2D Library Demo
 For CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

unit ribbonfrm;

  {$mode objfpc}{$H+}

interface

uses
  LResources, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, orca_scene2d;

type
  TfrmRibbon = class(TForm)
    d2Scene1: TD2Scene;
    Root1: TD2Background;
    skinRibbon: TD2Resources;
    Wnd: TD2HudWindow;
    RibbonButton: TD2HudButton;
    Text1: TD2Text;
    Image1: TD2Image;
    HudTabControl1: TD2HudTabControl;
    HudTabItem1: TD2HudTabItem;
    Layout1: TD2Layout;
    HudTabItem2: TD2HudTabItem;
    Layout2: TD2Layout;
    HudTabItem3: TD2HudTabItem;
    Layout3: TD2Layout;
    HudStatusBar1: TD2HudStatusBar;
    ColorBox1: TD2ColorBox;
    ColorPicker1: TD2ColorPicker;
    ColorQuad1: TD2ColorQuad;
    HudTabItem4: TD2HudTabItem;
    Layout4: TD2Layout;
    GlowEffect1: TD2GlowEffect;
    HudButton2: TD2HudButton;
    RibbonPopup: TD2Popup;
    Rectangle1: TD2Rectangle;
    ShadowEffect1: TD2ShadowEffect;
    HudButton1: TD2HudButton;
    HudSpeedButton1: TD2HudSpeedButton;
    HudSpeedButton2: TD2HudSpeedButton;
    HudSpeedButton3: TD2HudSpeedButton;
    HudSpeedButton4: TD2HudSpeedButton;
    HudSpeedButton5: TD2HudSpeedButton;
    Text2: TD2Text;
    Rectangle3: TD2Rectangle;
    Rectangle2: TD2Rectangle;
    Rectangle4: TD2Rectangle;
    HudLabel1: TD2HudLabel;
    HudSizeGrip1: TD2HudSizeGrip;
    HudAngleButton1: TD2HudAngleButton;
    HudCheckBox1: TD2HudCheckBox;
    HudHorzListBox1: TD2HudHorzListBox;
    HudGroupBox1: TD2HudGroupBox;
    HudGroupBox2: TD2HudGroupBox;
    Image2: TD2Image;
    Layout5: TD2Layout;
    HudSpeedButton6: TD2HudSpeedButton;
    HudSpeedButton7: TD2HudSpeedButton;
    HudSpeedButton8: TD2HudSpeedButton;
    HudTrackBar1: TD2HudTrackBar;
    HudTrackBar2: TD2HudTrackBar;
    HudLabel2: TD2HudLabel;
    HudLabel3: TD2HudLabel;
    Path1: TD2Path;
    ListBoxItem1: TD2ListBoxItem;
    ListBoxItem2: TD2ListBoxItem;
    ListBoxItem3: TD2ListBoxItem;
    ListBoxItem4: TD2ListBoxItem;
    ListBoxItem5: TD2ListBoxItem;
    ListBoxItem6: TD2ListBoxItem;
    ListBoxItem7: TD2ListBoxItem;
    ListBoxItem8: TD2ListBoxItem;
    ListBoxItem9: TD2ListBoxItem;
    HudPopupBox1: TD2HudPopupBox;
    HudStringComboBox1: TD2HudStringComboBox;
    Layout6: TD2Layout;
    HudMemo1: TD2HudMemo;
    HudPanel1: TD2HudPanel;
    HudTextBox1: TD2HudTextBox;
    HudTrack1: TD2HudTrack;
    HudScrollBar1: TD2HudScrollBar;
    HudRadioButton1: TD2HudRadioButton;
    HudStringListBox1: TD2HudStringListBox;
    procedure ColorQuad1Change(Sender: TObject);
    procedure RibbonButtonClick(Sender: TObject);
    procedure HudButton1Click(Sender: TObject);
    procedure HudTrackBar1Change(Sender: TObject);
    procedure HudTrackBar2Change(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmRibbon: TfrmRibbon;

implementation

{$R *.lfm}

procedure TfrmRibbon.ColorQuad1Change(Sender: TObject);
begin
  Wnd.Fill.SolidColor := ColorBox1.color;
end;

procedure TfrmRibbon.RibbonButtonClick(Sender: TObject);
begin
  RibbonPopup.Popup;
end;

procedure TfrmRibbon.HudButton1Click(Sender: TObject);
begin
  Application.Terminate;
end;

procedure TfrmRibbon.HudTrackBar1Change(Sender: TObject);
begin
  Wnd.Fill.SolidColor := (round(HudTrackBar1.Value * $FF) shl 24) or (Wnd.Fill.SolidColor and $FFFFFF);
  ColorQuad1.Alpha := HudTrackBar1.Value;
end;

procedure TfrmRibbon.HudTrackBar2Change(Sender: TObject);
begin
  Wnd.Opacity := HudTrackBar2.Value;
end;

end.
