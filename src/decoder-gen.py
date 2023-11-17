import argparse
import jinja2
import yaml

from pathlib import Path

def getArgs():
  parser = argparse.ArgumentParser(
    description="Tool to generate files according to ISA"
  )

  parser.add_argument(
    "-y", "--yaml", required=True, type=Path, help="ISA .yaml file"
  )

  parser.add_argument(
    "-o",
    "--output",
    required=True,
    type=Path,
    help="Output file",
  )

  parser.add_argument(
    "-j",
    "--jinja",
    required=True,
    type=Path,
    help="Jinja2 template file"
  )

  return parser.parse_args()

def main():
  args = getArgs()

  yml = None
  with open(args.yaml) as file:
    yml = yaml.safe_load(file)

  env = jinja2.Environment(loader=jinja2.FileSystemLoader(str(args.jinja.parent)))
  env.lstrip_blocks = True
  env.trim_blocks = True

  template = env.get_template(str(args.jinja.name))
  content = template.render(types=yml["types"])
  with open(args.output, "w") as file:
    file.write(content)

if "__main__" == __name__:
  main()
