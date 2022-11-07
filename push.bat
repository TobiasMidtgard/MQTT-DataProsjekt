git add .
set /P INPUT=Commit Melding: %=%
git commit -m %1 %INPUT%
git push