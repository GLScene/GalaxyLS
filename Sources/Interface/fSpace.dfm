object FormScene: TFormScene
  Left = 0
  Top = 0
  Caption = 'Browser GM'
  ClientHeight = 577
  ClientWidth = 847
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  Position = poScreenCenter
  OnCreate = FormCreate
  TextHeight = 13
  object StatusBar1: TStatusBar
    Left = 0
    Top = 558
    Width = 847
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
    Width = 709
    Height = 558
    Camera = GLCamera1
    Buffer.BackgroundColor = clBlack
    FieldOfView = 108.730621337890600000
    PenAsTouch = False
    Align = alClient
    TabOrder = 1
  end
  object PanelRight: TPanel
    Left = 709
    Top = 0
    Width = 138
    Height = 558
    Align = alRight
    TabOrder = 2
    object clbMethods: TCheckListBox
      Left = 6
      Top = 280
      Width = 121
      Height = 57
      ItemHeight = 13
      Items.Strings = (
        'Objects'
        'Delaunay Tetrahedra'
        'Voronoi Polyhedra'
        'Regular Grid ')
      TabOrder = 0
      OnClickCheck = clbMethodsClickCheck
    end
    object ClassGroup: TRadioGroup
      Left = 6
      Top = 7
      Width = 120
      Height = 218
      Caption = 'Object Classes'
      ItemIndex = 0
      Items.Strings = (
        'All'
        'O'
        'B'
        'A'
        'F'
        'G'
        'K'
        'M')
      TabOrder = 1
      OnClick = ClassGroupClick
    end
    object Panel2: TPanel
      Tag = 1
      Left = 75
      Top = 50
      Width = 41
      Height = 19
      Color = clHotLight
      ParentBackground = False
      TabOrder = 2
      OnClick = Panel2Click
    end
    object Panel1: TPanel
      Tag = 2
      Left = 75
      Top = 75
      Width = 41
      Height = 19
      Color = clSkyBlue
      ParentBackground = False
      TabOrder = 3
      OnClick = Panel2Click
    end
    object Panel3: TPanel
      Tag = 3
      Left = 75
      Top = 100
      Width = 41
      Height = 19
      Color = clBtnHighlight
      ParentBackground = False
      TabOrder = 4
      OnClick = Panel2Click
    end
    object Panel4: TPanel
      Tag = 4
      Left = 75
      Top = 125
      Width = 41
      Height = 19
      Color = clCream
      ParentBackground = False
      TabOrder = 5
      OnClick = Panel2Click
    end
    object Panel5: TPanel
      Tag = 5
      Left = 75
      Top = 150
      Width = 41
      Height = 19
      Color = clYellow
      ParentBackground = False
      TabOrder = 6
      OnClick = Panel2Click
    end
    object Panel6: TPanel
      Tag = 6
      Left = 75
      Top = 175
      Width = 41
      Height = 19
      Color = 35071
      ParentBackground = False
      TabOrder = 7
      OnClick = Panel2Click
    end
    object Panel7: TPanel
      Tag = 7
      Left = 75
      Top = 200
      Width = 41
      Height = 19
      Color = 17919
      ParentBackground = False
      TabOrder = 8
      OnClick = Panel2Click
    end
  end
  object GLScene1: TGLScene
    Left = 48
    Top = 16
    object GLCamera1: TGLCamera
      DepthOfView = 500000.000000000000000000
      FocalLength = 200.000000000000000000
      TargetObject = GLDummyCube1
      Position.Coordinates = {0000004000000040000000400000803F}
    end
    object GLDummyCube1: TGLDummyCube
      ShowAxes = True
      CubeSize = 400.000000000000000000
      EdgeColor.Color = {B1A8A83EB1A8A83EB1A8A83E0000803F}
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
      object DelStruct: TGLFreeForm
        Material.FrontProperties.Diffuse.Color = {EBE0E03EE4DB5B3F9A93133F0000803F}
      end
      object GLLines2: TGLLines
        Visible = False
        Nodes = <>
        Options = []
      end
      object VoxelDummy: TGLDummyCube
        CubeSize = 1.000000000000000000
      end
    end
  end
  object GLCadencer1: TGLCadencer
    Scene = GLScene1
    OnProgress = GLCadencer1Progress
    Left = 48
    Top = 104
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 192
    Top = 112
  end
  object MainMenu1: TMainMenu
    Left = 336
    Top = 16
    object Start1: TMenuItem
      Caption = 'File'
      OnClick = Start1Click
    end
    object Data1: TMenuItem
      Caption = 'Data'
      OnClick = Data1Click
    end
  end
  object OpenTextFileDialog1: TOpenTextFileDialog
    Left = 768
    Top = 88
  end
  object FDConnection1: TFDConnection
    Params.Strings = (
      'DriverID=SQLite')
    Left = 600
    Top = 16
  end
  object FDQuery1: TFDQuery
    Connection = FDConnection1
    Left = 600
    Top = 80
  end
  object FDPhysSQLiteDriverLink1: TFDPhysSQLiteDriverLink
    Left = 760
    Top = 16
  end
  object GLSimpleNavigation1: TGLSimpleNavigation
    Form = Owner
    GLSceneViewer = GLSceneViewer1
    FormCaption = 'Browser GM - %FPS'
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
    Left = 192
    Top = 16
  end
end
