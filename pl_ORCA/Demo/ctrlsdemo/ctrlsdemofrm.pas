unit ctrlsdemofrm;

{$MODE Delphi}

interface

uses     
  {$IFDEF FPC}
  LResources,
  {$ENDIF}
  SysUtils, Classes, Graphics, Controls, Forms,
  Dialogs,  Menus ,orca_scene2d;

type

  { TfrmCtrlsDemo }

  TfrmCtrlsDemo = class(TForm)
    ControlsScene: TD2Scene;
    Layout1: TD2Background;
    ScaleTrack: TD2TrackBar;
    Text1: TD2Label;
    Toolbar: TD2ToolBar;
    ControlRoot: TD2Layout;
    ScaleRoot: TD2Layout;
    Button1: TD2Button;
    Text2: TD2Label;
    Text3: TD2Label;
    ScrollBar1: TD2ScrollBar;
    ScrollBar2: TD2ScrollBar;
    Track1: TD2Track;
    Text4: TD2Label;
    ScrollBox1: TD2ScrollBox;
    TextScale: TD2Label;
    TabControl1: TD2TabControl;
    TabItem1: TD2TabItem;
    TabItem2: TD2TabItem;
    tabLayout2: TD2Layout;
    CheckBox1: TD2CheckBox;
    RadioButton1: TD2RadioButton;
    RadioButton2: TD2RadioButton;
    ProgressBar1: TD2ProgressBar;
    Label1: TD2Label;
    StringComboBox1: TD2StringComboBox;
    TabItem3: TD2TabItem;
    Label2: TD2Label;
    ListBoxItem1: TD2ListBoxItem;
    ListBoxItem2: TD2ListBoxItem;
    ListBoxItem3: TD2ListBoxItem;
    ListBoxItem4: TD2ListBoxItem;
    ListBoxItem6: TD2ListBoxItem;
    Label3: TD2Label;
    Label4: TD2Label;
    Image1: TD2Image;
    Label5: TD2Label;
    Path1: TD2Path;
    Label6: TD2Label;
    Button2: TD2Button;
    TextBox2: TD2TextBox;
    tabLayout3: TD2Layout;
    Rectangle1: TD2Rectangle;
    Text6: TD2Label;
    Ellipse1: TD2Ellipse;
    Label7: TD2Label;
    TreeView1: TD2TreeView;
    TreeViewItem1: TD2TreeViewItem;
    TreeViewItem2: TD2TreeViewItem;
    TreeViewItem3: TD2TreeViewItem;
    TreeViewItem4: TD2TreeViewItem;
    TreeViewItem5: TD2TreeViewItem;
    TreeViewItem6: TD2TreeViewItem;
    TreeViewItem7: TD2TreeViewItem;
    TreeViewItem8: TD2TreeViewItem;
    TreeViewItem9: TD2TreeViewItem;
    TreeViewItem10: TD2TreeViewItem;
    TreeViewItem11: TD2TreeViewItem;
    TreeViewItem12: TD2TreeViewItem;
    TreeViewItem13: TD2TreeViewItem;
    TreeViewItem14: TD2TreeViewItem;
    TreeViewItem15: TD2TreeViewItem;
    TreeViewItem16: TD2TreeViewItem;
    TreeViewItem17: TD2TreeViewItem;
    TreeViewItem18: TD2TreeViewItem;
    TreeViewItem19: TD2TreeViewItem;
    TreeViewItem20: TD2TreeViewItem;
    TreeViewItem21: TD2TreeViewItem;
    TreeViewItem22: TD2TreeViewItem;
    TreeViewItem23: TD2TreeViewItem;
    TreeViewItem24: TD2TreeViewItem;
    TreeViewItem25: TD2TreeViewItem;
    TreeViewItem26: TD2TreeViewItem;
    TreeViewItem27: TD2TreeViewItem;
    TreeViewItem28: TD2TreeViewItem;
    TreeViewItem29: TD2TreeViewItem;
    TreeViewItem30: TD2TreeViewItem;
    TreeViewItem31: TD2TreeViewItem;
    TreeViewItem32: TD2TreeViewItem;
    TreeViewItem33: TD2TreeViewItem;
    TreeViewItem34: TD2TreeViewItem;
    TreeViewItem35: TD2TreeViewItem;
    TreeViewItem36: TD2TreeViewItem;
    TreeViewItem37: TD2TreeViewItem;
    TabItem4: TD2TabItem;
    Layout3: TD2Layout;
    Expander1: TD2Expander;
    Label8: TD2Label;
    Button3: TD2Button;
    GroupBox1: TD2GroupBox;
    AniIndicator1: TD2AniIndicator;
    Button4: TD2Button;
    Image2: TD2Image;
    Button5: TD2Button;
    Label9: TD2Label;
    Image3: TD2Image;
    Label10: TD2Label;
    Label11: TD2Label;
    NumberBox1: TD2NumberBox;
    TrackBar1: TD2TrackBar;
    Label12: TD2Label;
    Button6: TD2Button;
    ListBox1: TD2ListBox;
    GlowEffect2: TD2GlowEffect;
    Label13: TD2Label;
    Label14: TD2Label;
    Memo1: TD2Memo;
    Label15: TD2Label;
    AngleButton1: TD2AngleButton;
    Label16: TD2Label;
    AngleButton2: TD2AngleButton;
    AngleButton3: TD2AngleButton;
    Label17: TD2Label;
    PopupBox1: TD2PopupBox;
    TextBox3: TD2TextBox;
    Rectangle2: TD2Panel;
    Splitter1: TD2Splitter;
    ModernStyle: TD2Resources;
    Label18: TD2Label;
    Panel1: TD2Panel;
    Label19: TD2Label;
    SpeedButton2: TD2SpeedButton;
    SpeedButton3: TD2SpeedButton;
    VistaStyle: TD2Resources;
    StyleBox: TD2PopupBox;
    externalStyle: TD2Resources;
    OpenDialog1: TOpenDialog;
    StatusBar1: TD2StatusBar;
    Label20: TD2Label;
    Panel2: TD2Panel;
    TabItem5: TD2TabItem;
    Layout2: TD2Layout;
    DropTarget1: TD2DropTarget;
    CloseButton1: TD2CloseButton;
    CloseButton2: TD2CloseButton;
    AirStyle: TD2Resources;
    TabItem6: TD2TabItem;
    Layout4: TD2Layout;
    StringListBox1: TD2StringListBox;
    ListTransform: TD2StringListBox;
    TrackBar2: TD2TrackBar;
    Label21: TD2Label;
    Label22: TD2Label;
    TrackBar3: TD2TrackBar;
    Ellipse2: TD2Ellipse;
    TextBox1: TD2TextBox;
    TextBox4: TD2TextBox;
    CompoundAngleBar1: TD2CompoundAngleBar;
    Panel3: TD2Panel;
    CompoundNumberBox1: TD2CompoundNumberBox;
    CompoundPopupBox1: TD2CompoundPopupBox;
    CompoundTextBox1: TD2CompoundTextBox;
    CompoundTrackBar1: TD2CompoundTrackBar;
    CompoundTrackBar2: TD2CompoundTrackBar;
    ToolButton1: TD2ToolButton;
    ToolPathButton1: TD2ToolPathButton;
    BitmapButton2: TD2BitmapButton;
    BitmapButton3: TD2BitmapButton;
    BitmapButton4: TD2BitmapButton;
    BitmapButton1: TD2BitmapButton;
    PathButton1: TD2PathButton;
    TabItem7: TD2TabItem;
    Layout5: TD2Layout;
    CornerButton1: TD2CornerButton;
    TrackBar4: TD2TrackBar;
    CheckBox3: TD2CheckBox;
    CheckBox4: TD2CheckBox;
    CheckBox5: TD2CheckBox;
    CheckBox6: TD2CheckBox;
    CornerButton2: TD2CornerButton;
    CornerButton3: TD2CornerButton;
    CornerButton4: TD2CornerButton;
    CornerButton5: TD2CornerButton;
    CornerButton6: TD2CornerButton;
    Path2: TD2Path;
    Path3: TD2Path;
    cornerList: TD2HorzListBox;
    btnHud: TD2ToolPathButton;
    btnAbout: TD2ToolButton;
    btnLoadStyle: TD2ToolButton;
    PathCheckBox1: TD2PathCheckBox;
    PathCheckBox2: TD2PathCheckBox;
    PathCheckBox3: TD2PathCheckBox;
    Label23: TD2Label;
    VertScrollBox1: TD2VertScrollBox;
    Button7: TD2Button;
    TrackBar5: TD2TrackBar;
    TextBox5: TD2TextBox;
    Expander2: TD2Expander;
    Expander3: TD2Expander;
    Expander4: TD2Expander;
    CompoundMemo1: TD2CompoundMemo;
    CompoundColorButton1: TD2CompoundColorButton;
    CompoundColorButton2: TD2CompoundColorButton;
    RoundButton1: TD2RoundButton;
    BlendStyle: TD2Resources;
    ListBox2: TD2ListBox;
    ListBoxItem5: TD2ListBoxItem;
    ListBoxItem7: TD2ListBoxItem;
    ListBoxItem8: TD2ListBoxItem;
    ListBoxItem9: TD2ListBoxItem;
    ListBoxItem10: TD2ListBoxItem;
    ListBoxItem11: TD2ListBoxItem;
    ListBoxItem12: TD2ListBoxItem;
    TreeView2: TD2TreeView;
    TreeViewItem38: TD2TreeViewItem;
    CircleButton1: TD2CircleButton;
    PopupButton1: TD2PopupButton;
    PopupMenu1: TPopupMenu;
    MenuItem1: TMenuItem;
    MenuItem2: TMenuItem;
    MenuItem3: TMenuItem;
    MenuItem4: TMenuItem;
    MenuItem5: TMenuItem;
    RoundTextBox1: TD2RoundTextBox;
    TabItem8: TD2TabItem;
    Layout6: TD2Layout;
    Memo2: TD2Memo;
    Memo3: TD2Memo;
    Label24: TD2Label;
    Label25: TD2Label;
    SpinBox1: TD2SpinBox;
    Label26: TD2Label;
    SmallScrollBar1: TD2SmallScrollBar;
    CheckBox7: TD2CheckBox;
    ExtremeStyle: TD2Resources;
    CheckBox2: TD2CheckBox;
    ComboTextBox1: TD2ComboTextBox;
    Label27: TD2Label;
    ComboTrackBar1: TD2ComboTrackBar;
    AlphaTrackBar1: TD2AlphaTrackBar;
    BWTrackBar1: TD2BWTrackBar;
    HueTrackBar1: TD2HueTrackBar;
    Label28: TD2Label;
    ComboColorBox1: TD2ComboColorBox;
    TextBoxClearBtn1: TD2TextBoxClearBtn;
    tabLayout1: TD2Layout;
    CalloutPanel1: TD2CalloutPanel;
    Label29: TD2Label;
    calloutTop: TD2RadioButton;
    calloutLeft: TD2RadioButton;
    calloutBottom: TD2RadioButton;
    calloutRight: TD2RadioButton;
    Calendar1: TD2Calendar;
    CalendarBox1: TD2CalendarBox;
    CalendarTextBox1: TD2CalendarTextBox;
    procedure FormShow(Sender: TObject);
    procedure ScaleTrackChange(Sender: TObject);
    procedure AngleButton1Change(Sender: TObject);
    procedure AngleButton3Change(Sender: TObject);
    procedure AngleButton2Change(Sender: TObject);
    procedure StyleBoxChange(Sender: TObject);
    procedure btnAboutClick(Sender: TObject);
    procedure btnHudClick(Sender: TObject);
    procedure btnLoadStyleClick(Sender: TObject);
    procedure TrackBar2Change(Sender: TObject);
    procedure TrackBar3Change(Sender: TObject);
    procedure TrackBar4Change(Sender: TObject);
    procedure CheckBox3Change(Sender: TObject);
    procedure CheckBox4Change(Sender: TObject);
    procedure CheckBox5Change(Sender: TObject);
    procedure CheckBox6Change(Sender: TObject);
    procedure cornerListChange(Sender: TObject);
    procedure CheckBox7Change(Sender: TObject);
    procedure CheckBox2Change(Sender: TObject);
    procedure calloutBottomChange(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
  end;

var
  frmCtrlsDemo: TfrmCtrlsDemo;

implementation

uses aboutboxfrm, hudctrls;

{$R *.lfm}

const
// CodeTyphon: Different platforms store resource files on different locations
{$IFDEF Windows}
  pathMedia = '..\..\styles\';
{$ELSE}
  pathMedia = '..\..\styles\';
{$ENDIF}

constructor TfrmCtrlsDemo.Create(AOwner: TComponent);
var
  i: integer;
begin
  inherited;
  for i := 0 to 50 do
    with TD2Rectangle.Create(Self) do
    begin
      parent := ScrollBox1;
      width := (30 + random(150));
      height := (30 + random(150));
      hittest := false;
      Position.x := random(1600);
      Position.y := random(1600);
      xRadius := random(20);
      yRadius := xRadius;
      fill.SolidColor := ((50 + random(205)) shl 24) or random($FFFFFF);
    end;
end;

destructor TfrmCtrlsDemo.Destroy;
begin
  inherited;
end;


procedure TfrmCtrlsDemo.ScaleTrackChange(Sender: TObject);
begin
  { change scale }
  ControlRoot.Scale.X := ScaleTrack.Value;
  ControlRoot.Scale.Y := ScaleTrack.Value;
  TextScale.Text := IntToStr(Round(ScaleTrack.Value * 100)) + '%';
end;

procedure TfrmCtrlsDemo.FormShow(Sender: TObject);
begin
    ModernStyle.FileName:=SetDirSeparators(ModernStyle.FileName);
    VistaStyle.FileName:=SetDirSeparators(VistaStyle.FileName);
    AirStyle.FileName:=SetDirSeparators(AirStyle.FileName);
    BlendStyle.FileName:=SetDirSeparators(BlendStyle.FileName);
    ExtremeStyle.FileName:=SetDirSeparators(ExtremeStyle.FileName);
end;

procedure TfrmCtrlsDemo.AngleButton1Change(Sender: TObject);
begin
  Label17.Text := IntToStr(Trunc(AngleButton1.Value));
end;

procedure TfrmCtrlsDemo.AngleButton3Change(Sender: TObject);
begin
  Label17.Text := IntToStr(Trunc(AngleButton3.Value));
end;

procedure TfrmCtrlsDemo.AngleButton2Change(Sender: TObject);
begin
  Label17.Text := IntToStr(Trunc(AngleButton2.Value));
end;

procedure TfrmCtrlsDemo.StyleBoxChange(Sender: TObject);
begin
  case StyleBox.ItemIndex of
    0: ControlsScene.Style := nil;
    1: ControlsScene.Style := ModernStyle;
    2: ControlsScene.Style := VistaStyle;
    3: ControlsScene.Style := AirStyle;
    4: ControlsScene.Style := BlendStyle;
    5: ControlsScene.Style := ExtremeStyle;
  end;
end;

procedure TfrmCtrlsDemo.btnAboutClick(Sender: TObject);
begin
  frmAbout := TfrmAbout.Create(Self);
  frmAbout.ShowModal;
  frmAbout.Free;
end;

procedure TfrmCtrlsDemo.btnHudClick(Sender: TObject);
begin
  frmLayerDemo := TfrmLayerDemo.Create(Self);
  frmLayerDemo.ShowModal;
  frmLayerDemo.Free;
end;

procedure TfrmCtrlsDemo.btnLoadStyleClick(Sender: TObject);
begin

  if OpenDialog1.FileName='' then  OpenDialog1.InitialDir:=SetDirSeparators(pathMedia);

  if OpenDialog1.Execute then
  begin
    externalStyle.Resource.LoadFromFile(OpenDialog1.FileName);
    ControlsScene.Style := externalStyle;
  end;
end;

procedure TfrmCtrlsDemo.TrackBar2Change(Sender: TObject);
begin
  ListTransform.RotateAngle := TrackBar2.Value;
  TextBox4.RotateAngle := TrackBar2.Value;
end;

procedure TfrmCtrlsDemo.TrackBar3Change(Sender: TObject);
begin
  ListTransform.Opacity := TrackBar3.Value;
  TextBox4.Opacity := TrackBar3.Value;
end;

procedure TfrmCtrlsDemo.TrackBar4Change(Sender: TObject);
begin
  CornerButton1.xRadius := TrackBar4.Value;
  CornerButton1.yRadius := TrackBar4.Value;

  CornerButton2.xRadius := TrackBar4.Value;;
  CornerButton2.yRadius := TrackBar4.Value;;
  CornerButton3.xRadius := TrackBar4.Value;;
  CornerButton3.yRadius := TrackBar4.Value;;
  CornerButton4.xRadius := TrackBar4.Value;;
  CornerButton4.yRadius := TrackBar4.Value;;

  CornerButton5.xRadius := TrackBar4.Value;;
  CornerButton5.yRadius := TrackBar4.Value;;
  CornerButton6.xRadius := TrackBar4.Value;;
  CornerButton6.yRadius := TrackBar4.Value;;
end;

procedure TfrmCtrlsDemo.CheckBox3Change(Sender: TObject);
begin
  if CheckBox3.IsChecked then
    CornerButton1.Corners := CornerButton1.Corners + [d2CornerBottomRight]
  else
    CornerButton1.Corners := CornerButton1.Corners - [d2CornerBottomRight]
end;

procedure TfrmCtrlsDemo.CheckBox4Change(Sender: TObject);
begin
  if CheckBox4.IsChecked then
    CornerButton1.Corners := CornerButton1.Corners + [d2CornerTopRight]
  else
    CornerButton1.Corners := CornerButton1.Corners - [d2CornerTopRight]
end;

procedure TfrmCtrlsDemo.CheckBox5Change(Sender: TObject);
begin
  if CheckBox5.IsChecked then
    CornerButton1.Corners := CornerButton1.Corners + [d2CornerBottomLeft]
  else
    CornerButton1.Corners := CornerButton1.Corners - [d2CornerBottomLeft]
end;

procedure TfrmCtrlsDemo.CheckBox6Change(Sender: TObject);
begin
  if CheckBox6.IsChecked then
    CornerButton1.Corners := CornerButton1.Corners + [d2CornerTopLeft]
  else
    CornerButton1.Corners := CornerButton1.Corners - [d2CornerTopLeft]
end;

procedure TfrmCtrlsDemo.cornerListChange(Sender: TObject);
begin
  CornerButton1.CornerType := TD2CornerType(cornerList.ItemIndex);
  CornerButton2.CornerType := TD2CornerType(cornerList.ItemIndex);
  CornerButton3.CornerType := TD2CornerType(cornerList.ItemIndex);
  CornerButton4.CornerType := TD2CornerType(cornerList.ItemIndex);
  CornerButton5.CornerType := TD2CornerType(cornerList.ItemIndex);
  CornerButton6.CornerType := TD2CornerType(cornerList.ItemIndex);
end;

procedure TfrmCtrlsDemo.CheckBox7Change(Sender: TObject);
begin
  ListBox1.UseSmallScrollBars := CheckBox7.IsChecked;
  TreeView1.UseSmallScrollBars := CheckBox7.IsChecked;
end;

procedure TfrmCtrlsDemo.CheckBox2Change(Sender: TObject);
begin
  StringListBox1.MultiSelect := CheckBox2.IsChecked;
end;

procedure TfrmCtrlsDemo.calloutBottomChange(Sender: TObject);
begin
  if calloutLeft.IsChecked then
    CalloutPanel1.CalloutPosition := d2CalloutLeft;
  if calloutRight.IsChecked then
    CalloutPanel1.CalloutPosition := d2CalloutRight;
  if calloutTop.IsChecked then
    CalloutPanel1.CalloutPosition := d2CalloutTop;
  if calloutBottom.IsChecked then
    CalloutPanel1.CalloutPosition := d2CalloutBottom;
end;

end.
