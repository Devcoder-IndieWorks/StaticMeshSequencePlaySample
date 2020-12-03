using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ObjMaterialRenameTool
{
    class RenameTool
    {
        private static readonly string MtlPattern = @"*.mtl";
        private static readonly string ObjPattern = @"*.obj";
        private static readonly string MtlKey = @"newmtl";
        private static readonly string ObjKey = @"usemtl";

        //---------------------------------------------------------------------

        private string FolderPath;
        private string MaterialName;
        private string[] ObjFileList;
        private int NumOfGroup = 0;
        private int GroupIndex = 0;

        //---------------------------------------------------------------------

        public RenameTool( string InFolderPath, string InMaterialName, int InNumOfGroup )
        {
            FolderPath = InFolderPath;
            MaterialName = InMaterialName;
            NumOfGroup = InNumOfGroup;
        }

        public bool Do()
        {
            return UpdateAllFiles();
        }

        private bool UpdateAllFiles()
        {
            int fileCount = 0;
            // .obj 파일 목록을 얻어옴.
            ObjFileList = Directory.GetFiles( FolderPath, RenameTool.ObjPattern );
            if ( ObjFileList.Length > 0 ) {
                // .mtl 파일 목록으로 .mtl 파일내 material 이름을 변경 함.
                Directory.GetFiles( FolderPath, RenameTool.MtlPattern )
                    .OrderBy( f => new FileInfo( f ).Name )
                    //.Select( f => fileCount += UpdateFile( f, fileCount ) );
                    .ToList()
                    .ForEach(f => fileCount += UpdateFile(f, fileCount));
            }

            return (fileCount > 0);
        }

        private int UpdateFile( string InFileName, int InFileCount )
        {
            if ( (InFileCount % NumOfGroup) == 0 )
                ++GroupIndex;

            bool replaced = false;
            // Step1: .mtl 파일내 material 이름 변경.
            if ( File.Exists( InFileName ) ) {
                var lines = File.ReadAllLines( InFileName );
                // 파일에서 material 이름이 있는 곳을 찾음.
                var pos = Array.FindIndex( lines, row => row.Contains( RenameTool.MtlKey ) );
                if ( pos >= 0 ) {
                    var replaceStr = new StringBuilder( RenameTool.MtlKey )
                        .Append( " " )
                        .Append( MaterialName )
                        .Append( GroupIndex.ToString() )
                        .ToString();
                    lines[ pos ] = replaceStr;
                    File.WriteAllLines( InFileName, lines );

                    replaced = true;
                }
            }

            // Step2: .obj 파일내 material 이름 변경.
            if ( replaced  ){
                replaced = false;
                // .mtl 파일 이름에서 .obj 파일 이름 중 겹치는 부분을 찾아내고, 찾아낸 문자열을 이용하여 연관된 .obj 파일을 .obj파일 목록에서 찾음.
                var subStr = (InFileName.Split( '-' )[1]).Split( '.' )[0];
                var objFile = ObjFileList.Where( f => f.Contains( subStr ) ).First();
                if ( File.Exists( objFile ) ) {
                    var lines = File.ReadAllLines( objFile );
                    // 파일에서 material 이름이 있는 곳을 찾음.
                    var pos = Array.FindIndex( lines, row => row.Contains( RenameTool.ObjKey ) );
                    if ( pos >= 0 ) {
                        var replaceStr = new StringBuilder( RenameTool.ObjKey )
                            .Append( " " )
                            .Append( MaterialName )
                            .Append( GroupIndex.ToString() )
                            .ToString();
                        lines[ pos ] = replaceStr;
                        File.WriteAllLines( objFile, lines );

                        replaced = true;
                    }
                }
            }

            return replaced ? 1 : 0;
        }
    }
}
