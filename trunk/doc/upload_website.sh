
#cd website; scp -r . takev@shell.sourceforge.net:/home/groups/c/ca/camvox/htdocs
./generate_website

rsync -rvpz --copy-unsafe-links --delete --delete-excluded --exclude='*.graffle' --exclude=.svn --rsh="ssh -l takev" website/ shell.sourceforge.net:/home/groups/c/ca/camvox/htdocs/

