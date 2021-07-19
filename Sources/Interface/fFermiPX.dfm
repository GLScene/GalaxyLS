object FormScene: TFormScene
  Left = 0
  Top = 0
  Caption = 'Fermi Paradox Simulator'
  ClientHeight = 525
  ClientWidth = 793
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar1: TStatusBar
    Left = 0
    Top = 506
    Width = 793
    Height = 19
    Panels = <
      item
        Text = 'Total points:'
        Width = 250
      end
      item
        Text = 'FPS:'
        Width = 50
      end>
  end
  object GLSceneViewer1: TGLSceneViewer
    Left = 0
    Top = 0
    Width = 655
    Height = 506
    Camera = GLCamera1
    Buffer.BackgroundColor = clBlack
    FieldOfView = 157.641555786132800000
    PenAsTouch = False
    Align = alClient
    OnMouseDown = GLSceneViewer1MouseDown
    TabOrder = 1
    ExplicitLeft = -6
  end
  object PanelRight: TPanel
    Left = 655
    Top = 0
    Width = 138
    Height = 506
    Align = alRight
    TabOrder = 2
    DesignSize = (
      138
      506)
    object CheckListBox1: TCheckListBox
      Left = 17
      Top = 0
      Width = 104
      Height = 113
      Anchors = [akTop, akRight]
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = 15
      Font.Name = 'Tahoma'
      Font.Style = []
      ItemHeight = 15
      Items.Strings = (
        '"O"'
        '"A"'
        '"B"'
        '"F"'
        '"G"'
        '"K"'
        '"M"')
      ParentFont = False
      ParentShowHint = False
      ShowHint = False
      TabOrder = 0
    end
  end
  object GLScene1: TGLScene
    Left = 40
    Top = 8
    object GLCamera1: TGLCamera
      DepthOfView = 200.000000000000000000
      FocalLength = 50.000000000000000000
      TargetObject = GLDummyCube1
      Position.Coordinates = {0000004000000040000000400000803F}
    end
    object GLDummyCube1: TGLDummyCube
      ShowAxes = True
      CubeSize = 1.000000000000000000
      VisibleAtRunTime = True
      object GLLines1: TGLLines
        Visible = False
        NodeColor.Color = {0000803F0000803F0000803F0000803F}
        Nodes = <>
        Options = []
      end
      object GLPoints1: TGLPoints
        Visible = False
        NoZWrite = False
        Static = False
      end
      object GLPolygon1: TGLPolygon
        Nodes = <>
      end
      object GLTetrahedron1: TGLTetrahedron
        Material.Texture.BorderColor.Color = {9A93133FE4DB5B3FEBE0E03E0000803F}
        Direction.Coordinates = {000000000000803F0000000000000000}
        Position.Coordinates = {0000803F0000803F0000803F0000803F}
        Up.Coordinates = {8B8F67BF000000004A51DABE00000000}
        Visible = False
      end
      object GLFreeForm1: TGLFreeForm
        Material.FrontProperties.Diffuse.Color = {EBE0E03EE4DB5B3F9A93133F0000803F}
      end
      object GLXYZGrid1: TGLXYZGrid
        LineColor.Color = {938C0C3E938E0E3FDCD6D63E0000803F}
        XSamplingScale.Min = -1.000000000000000000
        XSamplingScale.Max = 1.000000000000000000
        XSamplingScale.Origin = -1.000000000000000000
        XSamplingScale.Step = 0.100000001490116100
        YSamplingScale.Min = -1.000000000000000000
        YSamplingScale.Max = 1.000000000000000000
        YSamplingScale.Origin = -1.000000000000000000
        YSamplingScale.Step = 0.100000001490116100
        ZSamplingScale.Min = -1.000000000000000000
        ZSamplingScale.Max = 1.000000000000000000
        ZSamplingScale.Origin = -1.000000000000000000
        ZSamplingScale.Step = 0.100000001490116100
        Parts = [gpX, gpY, gpZ]
      end
    end
    object GLLightSource1: TGLLightSource
      ConstAttenuation = 1.000000000000000000
      Position.Coordinates = {0000000000000000000040400000803F}
      LightStyle = lsParallel
      Specular.Color = {0000803F0000803F0000803F0000803F}
      SpotCutOff = 180.000000000000000000
    end
  end
  object GLCadencer1: TGLCadencer
    Scene = GLScene1
    Left = 240
    Top = 16
  end
  object Timer1: TTimer
    Left = 336
    Top = 16
  end
  object MainMenu1: TMainMenu
    Left = 136
    Top = 8
    object mnFile: TMenuItem
      Caption = 'File'
    end
    object Mode1: TMenuItem
      Caption = 'Mode'
      object Points1: TMenuItem
        Caption = 'Points'
        RadioItem = True
      end
      object Delaunay1: TMenuItem
        Caption = 'Delaunay'
        RadioItem = True
      end
      object Voronoi1: TMenuItem
        Caption = 'Voronoi'
        RadioItem = True
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'Exit'
      end
    end
    object Data1: TMenuItem
      Caption = 'Data'
    end
    object ools1: TMenuItem
      Caption = 'Tools'
      object Options1: TMenuItem
        Caption = 'Options...'
        OnClick = Options1Click
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      object About1: TMenuItem
        Caption = 'About'
      end
    end
  end
  object OpenTextFileDialog1: TOpenTextFileDialog
    Left = 432
    Top = 16
  end
  object FDConnection1: TFDConnection
    Params.Strings = (
      
        'Database=C:\Users\dgoni\Documents\Embarcadero\Studio\STARS_DB\DA' +
        'TA\A_Delaunay.sqlite'
      'DriverID=SQLite')
    Left = 40
    Top = 72
  end
  object FDQuery1: TFDQuery
    Connection = FDConnection1
    Left = 136
    Top = 72
  end
  object FDPhysSQLiteDriverLink1: TFDPhysSQLiteDriverLink
    Left = 40
    Top = 144
  end
  object GLSimpleNavigation1: TGLSimpleNavigation
    Form = Owner
    GLSceneViewer = GLSceneViewer1
    FormCaption = 'Fermi Paradox Simulator - %FPS'
    KeyCombinations = <
      item
        ShiftState = [ssLeft, ssRight]
        Action = snaZoom
      end
      item
        ShiftState = [ssLeft]
        Action = snaMoveAroundTarget
      end
      item
        ShiftState = [ssRight]
        Action = snaMoveAroundTarget
      end>
    Left = 560
    Top = 16
  end
end
