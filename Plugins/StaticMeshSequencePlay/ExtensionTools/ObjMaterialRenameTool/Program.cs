using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ObjMaterialRenameTool
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine( "////////////////////////////////////////////////////////////////////////////////" );
            Console.WriteLine( "// Title:     Rename Tool for Obj File Material                               //" );
            Console.WriteLine( "// CopyRight: By Devcoder                                                     //" );
            Console.WriteLine( "////////////////////////////////////////////////////////////////////////////////" );
            Console.WriteLine( "" );

            if ( args.Length < 3 ) {
                Console.WriteLine( "Failure. [Obj의 Material 파일들이 있는 폴더, 변경할 Material 이름, 그룹당 Material 갯수를 지정 해야 함.]" );
                return;
            }

            Console.WriteLine( "Rename Tool Running..." );
            Console.WriteLine( "" );
            Console.WriteLine( "" );

            var folderPath = args[0];
            var replaceName = args[1];
            var numOfGroup = int.Parse( args[2] );
            var renameTool = new RenameTool( folderPath, replaceName, numOfGroup );
            if ( renameTool.Do() )
                Console.WriteLine( "Success. Renamed materials." );
            else
                Console.WriteLine( "Failure. Renamed materials." );

            Console.WriteLine( "" );
            Console.WriteLine( "" );
            Console.WriteLine( "Rename Tool Terminate..." );
        }
    }
}
