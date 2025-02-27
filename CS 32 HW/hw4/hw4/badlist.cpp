void removeBad(list<Movie*>& li)
{
    for (list<Movie*>::iterator it = li.begin(); it != li.end(); it++)
        if ((*it)->rating() <= 50)
        {
            delete *it;
            it = li.erase(it);
        }
}
