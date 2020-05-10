using System;
using System.Collections.Generic;
using System.Linq;
using static System.Console;

namespace BSSD_Lab5
{
    class Program
    {
        //filename to (attributes,ownerID, group)
        static Dictionary<string, (int,int,string)> Filesystem = new Dictionary<string, (int, int, string)>();

        //user ID to name
        static Dictionary<int, string> Users = new Dictionary<int, string>() { [0]="root" };

        //group name to users IDs
        static Dictionary<string, HashSet<int>> Groups = new Dictionary<string, HashSet<int>>() {["root"] = new HashSet<int>() { 0 },["users"] = new HashSet<int>() };

        //current user ID, default - root
        static int CurrentUser = 0;

        static int LastCreatedUserID = 0;

        static void Main(string[] args)
        {
            while(true)
            {
                var command = ReadLine();
                ProcessCommand(command);
                Write(CurrentUser == 0 ? "root# " : Users[CurrentUser] + "$ ");
            }
        }

        static void ProcessCommand(string command)
        {
            var input = command.Split(' ', ':').Where(s => s!= " ").ToArray();
            if (input[0] == string.Empty) return;
            switch (input[0])
            {
                case "adduser":
                    if (input.Length == 2 && !Users.ContainsValue(input[1]))
                    {
                        Users.Add(++LastCreatedUserID, input[1]);
                        Groups["users"].Add(LastCreatedUserID);
                        WriteLine($"User {input[1]} added.");
                    }
                    else
                        WriteLine("Error");
                    break;
                case "groupadd":
                    if (input.Length == 2 && !Groups.ContainsKey(input[1]))
                    {
                        Groups.Add(input[1], new HashSet<int>());
                        WriteLine($"Group {input[1]} added.");
                    }
                    else
                        WriteLine("Error");
                    break;
                case "usermod":
                    if (!Users.ContainsValue(input[4]))
                    {
                        WriteLine("Error");
                        break;
                    }
                    if (input.Length == 5 && input[1] == "-a" && input[2] == "-G" && Groups.ContainsKey(input[3]))
                    {
                        var uid = Users.First(u => u.Value == input[4]).Key;
                        //Groups.Where(g => g.Value.Contains(uid)).First().Value.Remove(uid);
                        Groups[input[3]].Add(uid);
                        WriteLine($"User {input[4]} added to group {input[3]}.");
                    }
                    else
                        WriteLine("Error");
                    break;
                case "su":
                    if (input.Length == 2 && Users.ContainsValue(input[1]))
                    {
                        var uid = Users.First(u => u.Value == input[1]).Key;
                        CurrentUser = uid;
                        WriteLine($"Current user changed to {input[1]}.");
                    }
                    else
                        WriteLine("Error");
                    break;
                case "touch":
                    if (input.Length == 2)
                        if (Filesystem.TryAdd(input[1], (640, CurrentUser, Groups.Where(g => g.Value.Contains(CurrentUser)).First().Key)))
                            WriteLine($"File {input[1]} created with attributes {640}.");
                        else
                            WriteLine($"File {input[1]} already exist.");
                    else
                        WriteLine("Error");
                    break;
                case "read":
                    if (input.Length == 2 && Filesystem.ContainsKey(input[1]))
                        WriteLine(CheckRights(input[1], Rights.Read) ? "Permission confirmed." : "Permission denied.");
                    else
                        WriteLine("Error");
                    break;
                case "exec":
                    if (input.Length == 2 && Filesystem.ContainsKey(input[1]))
                        WriteLine(CheckRights(input[1], Rights.Exec) ? "Permission confirmed." : "Permission denied.");
                    else
                        WriteLine("Error");
                    break;
                case "write":
                    if (input.Length == 2 && Filesystem.ContainsKey(input[1]))
                        WriteLine(CheckRights(input[1], Rights.Write) ? "Permission confirmed." : "Permission denied.");
                    else
                        WriteLine("Error");
                    break;
                case "chmod":
                    {
                        if (input.Length == 3 && Filesystem.ContainsKey(input[2]) &&
                            int.TryParse(input[1], out int mask) && IsCoorectMask(mask) &&//mask correct
                            (CurrentUser == 0 || CurrentUser == Filesystem[input[2]].Item2))//has rights
                        {
                            Filesystem[input[2]] = (mask, Filesystem[input[2]].Item2, Filesystem[input[2]].Item3);//set new
                            WriteLine($"New attributes {mask} are set for file {input[2]}");
                        }
                        else
                            WriteLine("Error");
                    }
                    break;
                case "chown":
                    {
                        if (input.Length == 4 && Users.ContainsValue(input[1]) && Groups.ContainsKey(input[2]) && Filesystem.ContainsKey(input[3]) &&
                            (CurrentUser == 0 || CurrentUser == Filesystem[input[3]].Item2))//has rights
                        {
                            Filesystem[input[3]] = (Filesystem[input[3]].Item1, Users.First(u => u.Value == input[1]).Key, input[2]);//set new
                            WriteLine($"New group {input[2]} and user {input[1]} are set for file {input[3]}");
                        }
                        else
                            WriteLine("Error");
                    }
                    break;
                default:
                    WriteLine("Unknown command.");
                    break;
            }
        }

        enum Rights { Read = 0, Write = 1, Exec = 2 };

        /// <summary>
        /// Checks if <see cref="CurrentUser"/> has <paramref name="rights"/> to <paramref name="filename"/>
        /// </summary>
        /// <param name="filename">The required file name</param>
        /// <param name="rights">Requestes right</param>
        /// <returns></returns>
        static bool CheckRights(string filename, Rights rights)
        {
            if (CurrentUser == 0)// current user is root
                return true;
            if (Filesystem[filename].Item2 == CurrentUser)// current user is owner
                return ((((Filesystem[filename].Item1 / 100) % 10) >> (2 - (int)rights)) & 1) == 1;
            if (Groups[Filesystem[filename].Item3].Contains(CurrentUser))// current user is in group owner
                return ((((Filesystem[filename].Item1 / 10) % 10) >> (2 - (int)rights)) & 1) == 1;
            //currnt user is other
                return ((((Filesystem[filename].Item1 / 1) % 10) >> (2 - (int)rights)) & 1) == 1;
        }
        static int[] Pow10 = { 1, 10, 100 };

        static bool IsCoorectMask(int mask)
        {
            if (mask >= 1000) return false;
            return ((mask / 100) % 10) <= 7 && ((mask / 10) % 10) <= 7 && ((mask / 1) % 10) <= 7;
        }
    }
}
