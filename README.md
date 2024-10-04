## Create Your Repository
```bash
git clone git@github.com:Dr-Rank/GMASExSKGv2.git MyShooterGame
cd MyShooterGame
git remote rename origin upstream
git remote add origin git@github.com:your-account/your-repo.git # Replace with your repository url
git push -u origin main
```

## Merging Updates
```bash
git fetch upstream
git merge upstream/main
```
