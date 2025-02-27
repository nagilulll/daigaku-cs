void removeBad(vector<Movie*>& v)
{
    for (vector<Movie*>::iterator it = v.begin(); it != v.end();)
    {
        if ((*it)->rating() <= 50)
        {
            delete *it;
            it = v.erase(it);
        }
        else
            it++;
    }
}
