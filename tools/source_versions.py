"""Select regional C source for inspection tools without expanding C macros."""

from pathlib import Path
import re


def region_source(source, version="us"):
    """Resolve VERSION_US/JP guards, retaining other preprocessor directives.

    Mask excluded characters instead of removing them so source offsets stay valid.
    This is for source inspection; compilation still uses the C preprocessor.
    """
    if version not in ("us", "jp"):
        raise ValueError("version must be us or jp")
    stack = []
    active = True
    output = []
    for line in source.splitlines(True):
        directive = re.match(r"\s*#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)", line)
        emit = active
        if directive:
            op, expression = directive.groups()
            expression = re.sub(r"/\*.*?\*/|//.*", "", expression).strip()
            match = re.fullmatch(r"(!\s*)?(?:defined\s*\(\s*(VERSION_US|VERSION_JP)\s*\)|defined\s+(VERSION_US|VERSION_JP)|(VERSION_US|VERSION_JP))", expression)
            known = match is not None
            condition = True
            if known:
                macro = next(x for x in match.groups()[1:] if x)
                condition = macro == "VERSION_" + version.upper()
                if match[1] or op == "ifndef":
                    condition = not condition
            if op in ("if", "ifdef", "ifndef"):
                stack.append([known, active, condition])
                if known:
                    emit = False
                    active = active and condition
            elif op == "else" and stack:
                regional, parent, chosen = stack[-1]
                if regional:
                    emit = False
                    active = parent and not chosen
            elif op == "elif" and stack and stack[-1][0]:
                if not known:
                    raise ValueError("mixed regional and non-regional #elif")
                regional, parent, chosen = stack[-1]
                emit = False
                active = parent and not chosen and condition
                stack[-1][2] = chosen or condition
            elif op == "endif" and stack:
                regional, parent, chosen = stack.pop()
                emit = parent and not regional
                active = parent
        output.append(line if emit else re.sub(r"[^\n]", " ", line))
    if stack:
        raise ValueError("unclosed source conditional")
    return "".join(output)


def read_source(path, version="us"):
    return region_source(Path(path).read_text(), version)
