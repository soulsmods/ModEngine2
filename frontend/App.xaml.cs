using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Configuration;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;

namespace ModEngine2
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        public ObservableCollection<Game> Games { get; set; } = new ObservableCollection<Game>();

        private void AppStartup(object sender, StartupEventArgs args)
        {
            LoadGames();
        }

        private void LoadGames()
        {
            Games.Add(new Game(Game.PrettyGameName(GameType.DarkSoulsIII), GameType.DarkSoulsIII, $@"E:\SteamLibrary\steamapps\common\DARK SOULS III\Game"));
            Games.Add(new Game(Game.PrettyGameName(GameType.Sekiro), GameType.Sekiro, $@"E:\SteamLibrary\steamapps\common\Sekiro"));
        }
    }
}
