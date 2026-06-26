# Vibematching

There are currently enough unmatched 'easy' functions in the project that you can probably help out without knowing anything about decompilation. This is a terrible way to learn decompilation but a great way for a Snowboard Kids fan with unspent tokens to help out.

## Requirements

1. System requirements from README.md; and
2. Access to Claude or Codex (other models will probably work but probably not with Nigel, see the 'I hate Nigel' section for more information).

## Setup

1. Follow the setup instructions in README.md and ensure that you can extract/build the rom; and
2. Install [Nigel](https://github.com/cdlewis/nigel).

## Matching

Go to the root directory of the snowboardkids-decomp Git repository and run:

```
nigel decompile-unmatched
```

Or if you're using Codex:

```
nigel decompile-unmatched --claude-command="codex" --claude-flags="--sandbox=\"danger-full-access\""
```

## Avoiding Conflicts

Be sure to put up pull requests with matched functions often to avoid people potentially repeating/preempting your work.

You can use Nigel's --shard option to split work between multiple people. For example:

```
# Person 1 runs:
nigel decompile-unmatched --shard 1/3

# Person 2 runs:
nigel decompile-unmatched --shard 2/3

# Person 3 runs:
nigel decompile-unmatched --shard 3/3
```

## I hate Nigel

If you don't want to use Nigel you can achieve a similar result by prompting your agent with something along the lines of:

```
Your job is to match all unmatched functions in this project. 

To find out the best candidate function to decompile, run:

    python3 tools/score_functions.py asm/nonmatchings

Pass that function to a subagent with instructions to:

    decompile the function: $FUNCTION_NAME

Periodically check on the sub-agent, when it's done ensure it has committed its work. If it was unable to decompile the function, add the function name to `tools/difficult_functions` so it will be skipped in the future.

Each time the sub-agent finishes, use score_functions.py to find a new function and repeat the matching process with a new sub-agent.
```