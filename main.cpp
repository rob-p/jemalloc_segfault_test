/**
MWE for salmon segfault
**/

#include <boost/thread/thread.hpp>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>

bool verbose = false;

int main(int argc, char* argv[]) {
  using std::string;
  namespace po = boost::program_options;
  std::setlocale(LC_ALL, "en_US.UTF-8");

  // With no arguments, print help
  if (argc == 1) {
    std::vector<std::string> o;
    std::cout << "HELP\n";
    std::exit(1);
  }

  try {

    // subcommand parsing code inspired by :
    // https://gist.github.com/randomphrase/10801888
    po::options_description sfopts("Allowed Options");
    sfopts.add_options()("version,v", "print version string")(
        "no-version-check",
        "don't check with the server to see if this is the latest version")(
        "cite,c", "show citation information")(
        "help,h", "produce help message")("command", po::value<string>(),
                                          "command to run {index, quant, sf}")(
        "subargs", po::value<std::vector<std::string>>(),
        "Arguments for command");

    //po::options_description all("Allowed Options");
    //all.add(sfopts);

    po::positional_options_description pd;
    pd.add("command", 1).add("subargs", -1);

    po::variables_map vm;
    po::parsed_options parsed = po::command_line_parser(argc, argv)
                                    .options(sfopts)
                                    .positional(pd)
                                    .allow_unregistered()
                                    .run();
    po::store(parsed, vm);

    if (vm.count("version")) {
      std::cout << "salmon V1\n";
      std::exit(0);
    }

    if (vm.count("help") and !vm.count("command")) {
      std::vector<std::string> o;
      std::cout << "HELP!!!\n";
      std::exit(0);
    }

    if (vm.count("cite") and !vm.count("command")) {
      std::cout << "cite\n";
      std::exit(0);
    }

    if (!vm.count("no-version-check")) {
      //std::string versionMessage = getVersionMessage();
      std::cout << "VERSION";
    }

    // po::notify(vm);

    std::string cmd = vm["command"].as<std::string>();
    std::vector<std::string> opts =
        po::collect_unrecognized(parsed.options, po::include_positional);
    opts.erase(opts.begin());
    // if there was a help and a command, then add the help back since it was
    // parsed
    if (vm.count("help")) {
      opts.insert(opts.begin(), "--help");
    }

    /*
    std::unordered_map<string, std::function<int(int, const char* [])>> cmds(
        {{"index", salmonIndex},
         {"quant", salmonQuantify},
         {"quantmerge", salmonQuantMerge},
         {"alevin", salmonBarcoding},
         {"swim", salmonSwim}});

    int32_t subCommandArgc = opts.size() + 1;
    std::unique_ptr<const char* []> argv2(new const char*[subCommandArgc]);
    argv2[0] = argv[0];
    for (int32_t i = 0; i < subCommandArgc - 1; ++i) {
      argv2[i + 1] = opts[i].c_str();
    }

    auto cmdMain = cmds.find(cmd);
    if (cmdMain == cmds.end()) {
      // help(subCommandArgc, argv2);
      return help(opts);
    } else {
      // If the command is quant; determine whether
      // we're quantifying with raw sequences or alignemnts
      if (cmdMain->first == "quant") {

        if (subCommandArgc < 2) {
          return dualModeMessage();
        }
        // detect mode-specific help request
        if (strncmp(argv2[1], "--help-alignment", 16) == 0) {
          std::vector<char> helpStr{'-', '-', 'h', 'e', 'l', 'p', '\0'};
          const char* helpArgv[] = {argv[0], &helpStr[0]};
          return salmonAlignmentQuantify(2, helpArgv);
        } else if (strncmp(argv2[1], "--help-reads", 12) == 0) {
          std::vector<char> helpStr{'-', '-', 'h', 'e', 'l', 'p', '\0'};
          const char* helpArgv[] = {argv[0], &helpStr[0]};
          return salmonQuantify(2, helpArgv);
        }

        // detect general help request
        if (strncmp(argv2[1], "--help", 6) == 0 or
            strncmp(argv2[1], "-h", 2) == 0) {
          return dualModeMessage();
        }

        // otherwise, detect and dispatch the correct mode
        bool useSalmonAlign{false};
        for (int32_t i = 0; i < subCommandArgc; ++i) {
          if (strncmp(argv2[i], "-a", 2) == 0 or
              strncmp(argv2[i], "--alignments", 12) == 0) {
            useSalmonAlign = true;
            break;
          }
        }
        if (useSalmonAlign) {
          return salmonAlignmentQuantify(subCommandArgc, argv2.get());
        } else {
          return salmonQuantify(subCommandArgc, argv2.get());
        }
      } else {
        return cmdMain->second(subCommandArgc, argv2.get());
      }
    }
    */

  } catch (po::error& e) {
    std::cerr << "Program Option Error (main) : [" << e.what()
              << "].\n Exiting.\n";
    std::exit(1);
  } catch (...) {
    std::cerr << argv[0] << " was invoked improperly.\n";
    std::cerr << "For usage information, try " << argv[0]
              << " --help\nExiting.\n";
  }

  return 0;
}
