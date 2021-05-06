using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ModEngine2
{
    public enum GameType
    {
        DarkSoulsRemastered,
        DarkSoulsIISOTFS,
        DarkSoulsIII,
        Sekiro,
    }

    public class Game
    {
        public string Name { get; set; }
        public GameType Type { get; set; }
        public string Path { get; set; }

        public static string PrettyGameName(GameType type)
        {
            switch (type)
            {
                case GameType.DarkSoulsRemastered:
                    return "Dark Souls Remastered";
                case GameType.DarkSoulsIISOTFS:
                    return "Dark Souls II: Scholar of the First Sin";
                case GameType.DarkSoulsIII:
                    return "Dark Souls III";
                case GameType.Sekiro:
                    return "Sekiro: Shadows Die Twice";
                default:
                    throw new Exception("Unhandled game type");
            }
        }

        public Game(string name, GameType type, string path)
        {
            Name = name;
            Type = type;
            Path = path;
        }

        public string ExecutableName
        { 
            get
            {
                switch (Type)
                {
                    case GameType.DarkSoulsRemastered:
                        return "DarkSoulsRemastered.exe";
                    case GameType.DarkSoulsIISOTFS:
                        return "DarkSoulsII.exe";
                    case GameType.DarkSoulsIII:
                        return "DarkSoulsIII.exe";
                    case GameType.Sekiro:
                        return "Sekiro.exe";
                    default:
                        throw new Exception("Unhandled game type");
                }
            }
        }

        public string ExecutablePath
        {
            get
            {
                return $@"{Path}\{ExecutableName}";
            }
        }
    }
}
